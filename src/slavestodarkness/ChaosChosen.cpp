/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosChosen.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 140 * 4;

    bool ChaosChosen::s_registered = false;

    Unit *ChaosChosen::Create(const ParameterList &parameters) {
        auto unit = new ChaosChosen();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool drummer = GetBoolParam("Drummer", parameters, false);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(numModels, iconBearer, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosChosen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosChosen::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosChosen::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Chosen", factoryMethod);
        }
    }

    ChaosChosen::ChaosChosen() :
            SlavesToDarknessBase("Chaos Chosen", 6, g_wounds, 7, 4, false),
            m_greataxe(Weapon::Type::Melee, "Soul Splitter", 1, 3, 3, 3, -1, 1),
            m_greataxeChampion(Weapon::Type::Melee, "Soul Splitter", 1, 4, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_CHOSEN};
        m_weapons = {&m_greataxe, &m_greataxeChampion};

        s_globalBraveryMod.connect(this, &ChaosChosen::iconBearer, &m_braverySlot);
    }

    ChaosChosen::~ChaosChosen() {
        m_braverySlot.disconnect();
    }

    bool ChaosChosen::configure(int numModels, bool iconBearer, bool drummer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_drummer = drummer;

        auto champion = new Model(g_basesize, wounds());
        champion->addMeleeWeapon(&m_greataxeChampion);
        champion->setName("Exalted Champion");
        addModel(champion);

        if (m_iconBearer) {
            auto model = new Model(g_basesize, wounds());
            model->setName("Icon Bearer");
            model->addMeleeWeapon(&m_greataxe);
            addModel(model);
        }

        if (m_drummer) {
            auto model = new Model(g_basesize, wounds());
            model->setName("Drummer");
            model->addMeleeWeapon(&m_greataxe);
            addModel(model);
        }

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_greataxe);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    void ChaosChosen::onWounded() {
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

    int ChaosChosen::runModifier() const {
        auto modifier = Unit::runModifier();
        if (m_drummer) modifier += 1;
        return modifier;
    }

    int ChaosChosen::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (m_drummer) modifier += 1;
        return modifier;
    }

    void ChaosChosen::onRestore() {
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

    Wounds ChaosChosen::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Soul splitter
        if ((hitRoll >= 6) && (weapon->name() == m_greataxe.name())) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ChaosChosen::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int ChaosChosen::iconBearer(const Unit *unit) {
        // Icon Bearer
        if (m_iconBearer && (unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 6.0)) {
            return -1;
        }
        return 0;
    }

} //SlavesToDarkness