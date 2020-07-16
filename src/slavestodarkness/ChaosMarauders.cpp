/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosMarauders.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool ChaosMarauders::s_registered = false;

    Unit *ChaosMarauders::Create(const ParameterList &parameters) {
        auto unit = new ChaosMarauders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, AxeAndShield);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool drummer = GetBoolParam("Drummer", parameters, false);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(numModels, weapons, iconBearer, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosMarauders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Flail, AxeAndShield};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", AxeAndShield, weapons),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Marauders", factoryMethod);
        }
    }

    ChaosMarauders::ChaosMarauders() :
            SlavesToDarknessBase("Chaos Marauders", 6, WOUNDS, 5, 6, false),
            m_axe(Weapon::Type::Melee, "Barbarian Axe", 1, 2, 4, 4, 0, 1),
            m_flail(Weapon::Type::Melee, "Barbarian Flail", 2, 1, 4, 3, 0, 1),
            m_axeChieftain(Weapon::Type::Melee, "Barbarian Axe", 1, 3, 4, 4, 0, 1),
            m_flailChieftain(Weapon::Type::Melee, "Barbarian Flail", 2, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_MARAUDERS};
        m_weapons = {&m_axe, &m_flail, &m_axeChieftain, &m_flailChieftain};
        m_battleFieldRole = Battleline;

        s_globalBraveryMod.connect(this, &ChaosMarauders::iconBearer, &m_connection);
    }

    ChaosMarauders::~ChaosMarauders() {
        m_connection.disconnect();
    }

    bool ChaosMarauders::configure(int numModels, WeaponOption weapons, bool iconBearer, bool drummer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_weaponOption = weapons;
        m_iconBearer = iconBearer;
        m_drummer = drummer;

        auto leader = new Model(BASESIZE, wounds());
        if (weapons == AxeAndShield) {
            leader->addMeleeWeapon(&m_axeChieftain);
        } else if (weapons == Flail) {
            leader->addMeleeWeapon(&m_flailChieftain);
        }
        leader->setName("Marauder Chieftain");
        addModel(leader);

        if (m_iconBearer) {
            auto model = new Model(BASESIZE, wounds());
            model->setName("Icon Bearer");
            if (weapons == AxeAndShield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            addModel(model);
        }

        if (m_drummer) {
            auto model = new Model(BASESIZE, wounds());
            model->setName("Drummer");
            if (weapons == AxeAndShield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            addModel(model);
        }

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == AxeAndShield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            addModel(model);
        }

        // Darkwood Shields
        if (m_weaponOption == WeaponOption::AxeAndShield) {
            m_save = 5;
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string ChaosMarauders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == AxeAndShield) {
                return "Barbarian Axe and Darkwood Shield";
            } else if (parameter.intValue == Flail) {
                return "Barbarian Flail";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosMarauders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Barbarian Axe and Darkwood Shield") {
            return AxeAndShield;
        } else if (enumString == "Barbarian Flail") {
            return Flail;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    void ChaosMarauders::onWounded() {
        Unit::onWounded();

        // Check for special models
        for (const auto &ip : m_models) {
            if (ip->slain() && (ip->getName() == "Drummer")) {
                m_drummer = false;
            }
            if (ip->slain() && (ip->getName() == "Icon Bearer")) {
                m_iconBearer = false;
            }
        }
    }

    int ChaosMarauders::runModifier() const {
        auto modifier = Unit::runModifier();
        if (m_drummer) modifier += 1;
        return modifier;
    }

    int ChaosMarauders::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (m_drummer) modifier += 1;
        return modifier;
    }

    void ChaosMarauders::onRestore() {
        Unit::onRestore();

        // Check for special models
        for (const auto &ip : m_models) {
            if (ip->getName() == "Drummer") {
                m_drummer = true;
            }
            if (ip->getName() == "Icon Bearer") {
                m_iconBearer = true;
            }
        }
    }

    int ChaosMarauders::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Barbarian Hordes
        auto modifier = Unit::toHitModifier(weapon, target);
        if (remainingModels() >= 10) modifier++;
        return modifier;
    }

    int ChaosMarauders::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Barbarian Hordes
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
        if (remainingModels() >= 20) rend--;
        return rend;
    }

    int ChaosMarauders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int ChaosMarauders::iconBearer(const Unit *unit) {
        // Icon Bearer
        if (m_iconBearer && (unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 6.0)) {
            return -1;
        }
        return 0;
    }

    int ChaosMarauders::rollChargeDistance() const {
        // Boundless Ferocity
        auto roll1 = Dice::rollD6();
        auto roll2 = Dice::rollD6();
        if (roll1 < roll2) roll1 = 6;
        else if (roll2 < roll1) roll2 = 6;
        else roll1 = 6;

        m_unmodifiedChargeRoll = roll1 + roll2;
        return m_unmodifiedChargeRoll + chargeModifier();
    }

} //SlavesToDarkness