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
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 320;

    bool ChaosMarauders::s_registered = false;

    Unit *ChaosMarauders::Create(const ParameterList &parameters) {
        auto unit = new ChaosMarauders();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Axe_And_Shield);
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
            static const std::array<int, 2> weapons = {Flail, Axe_And_Shield};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Axe_And_Shield, weapons),
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
            SlavesToDarknessBase("Chaos Marauders", 6, g_wounds, 5, 6, false),
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
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_weaponOption = weapons;

        auto leader = new Model(g_basesize, wounds());
        if (weapons == Axe_And_Shield) {
            leader->addMeleeWeapon(&m_axeChieftain);
        } else if (weapons == Flail) {
            leader->addMeleeWeapon(&m_flailChieftain);
        }
        leader->setName("Marauder Chieftain");
        addModel(leader);

        if (iconBearer) {
            auto model = new Model(g_basesize, wounds());
            model->setName(Model::IconBearer);
            if (weapons == Axe_And_Shield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            addModel(model);
        }

        if (drummer) {
            auto model = new Model(g_basesize, wounds());
            model->setName(Model::Drummer);
            if (weapons == Axe_And_Shield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            addModel(model);
        }

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Axe_And_Shield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            addModel(model);
        }

        // Darkwood Shields
        if (m_weaponOption == WeaponOption::Axe_And_Shield) {
            m_save = 5;
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string ChaosMarauders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Axe_And_Shield) {
                return "Barbarian Axe and Darkwood Shield";
            } else if (parameter.intValue == Flail) {
                return "Barbarian Flail";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosMarauders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Barbarian Axe and Darkwood Shield") {
            return Axe_And_Shield;
        } else if (enumString == "Barbarian Flail") {
            return Flail;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int ChaosMarauders::runModifier() const {
        auto modifier = Unit::runModifier();
        if (isNamedModelAlive(Model::Drummer)) modifier += 1;
        return modifier;
    }

    int ChaosMarauders::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Drummer)) modifier += 1;
        return modifier;
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int ChaosMarauders::iconBearer(const Unit *unit) {
        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer) && (unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 6.0)) {
            return -1;
        }
        return 0;
    }

    int ChaosMarauders::rollChargeDistance() const {
        // Boundless Ferocity
        auto roll1 = Dice::RollD6();
        auto roll2 = Dice::RollD6();
        if (roll1 < roll2) roll1 = 6;
        else if (roll2 < roll1) roll2 = 6;
        else roll1 = 6;

        m_unmodifiedChargeRoll = roll1 + roll2;
        return m_unmodifiedChargeRoll + chargeModifier();
    }

} //SlavesToDarkness