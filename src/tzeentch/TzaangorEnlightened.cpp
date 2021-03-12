/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorEnlightened.h>
#include <UnitFactory.h>
#include <Board.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 300;

    bool TzaangorEnlightened::s_registered = false;

    TzaangorEnlightened::TzaangorEnlightened() :
            TzeentchBase("Tzaangor Enlightened", 6, g_wounds, 6, 5, false),
            m_tzeentchianSpear(Weapon::Type::Melee, "Tzeentchian Spear", 2, 3, 4, 3, -1, 2),
            m_tzeentchianSpearAviarch(Weapon::Type::Melee, "Tzeentchian Spear", 2, 4, 4, 3, -1, 2),
            m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_ENLIGHTENED};
        m_weapons = {&m_tzeentchianSpear, &m_tzeentchianSpearAviarch, &m_viciousBeak};
    }

    bool TzaangorEnlightened::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        auto aviarch = new Model(g_basesize, wounds());
        aviarch->addMeleeWeapon(&m_tzeentchianSpearAviarch);
        aviarch->addMeleeWeapon(&m_viciousBeak);
        addModel(aviarch);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_tzeentchianSpear);
            model->addMeleeWeapon(&m_viciousBeak);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *TzaangorEnlightened::Create(const ParameterList &parameters) {
        auto *unit = new TzaangorEnlightened();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int)ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TzaangorEnlightened::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TzaangorEnlightened::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TzaangorEnlightened::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH, BEASTS_OF_CHAOS}
            };
            s_registered = UnitFactory::Register("Tzaangor Enlightened", factoryMethod);
        }
    }

    Rerolls TzaangorEnlightened::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Guided by the Past
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        bool enemyHasFought = false;
        for (auto ip : units) {
            enemyHasFought |= ip->hasFought();
        }
        if (enemyHasFought) return Rerolls::Failed;

        return TzeentchBase::toHitRerolls(weapon, target);
    }

    Rerolls TzaangorEnlightened::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Guided by the Past
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        bool enemyHasFought = false;
        for (auto ip : units) {
            enemyHasFought |= ip->hasFought();
        }
        if (enemyHasFought) return Rerolls::Failed;

        return TzeentchBase::toWoundRerolls(weapon, target);
    }

    int TzaangorEnlightened::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch