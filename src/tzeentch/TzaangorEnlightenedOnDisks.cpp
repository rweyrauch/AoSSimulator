/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorEnlightenedOnDisks.h>
#include <UnitFactory.h>
#include <Board.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;


    bool TzaangorEnlightenedOnDisks::s_registered = false;

    TzaangorEnlightenedOnDisks::TzaangorEnlightenedOnDisks() :
            TzeentchBase("Tzaangor Enlightened on Disks", 16, g_wounds, 6, 5, true),
            m_tzeentchianSpear(Weapon::Type::Melee, "Tzeentchian Spear", 2, 3, 4, 3, -1, 2),
            m_tzeentchianSpearAviarch(Weapon::Type::Melee, "Tzeentchian Spear", 2, 4, 4, 3, -1, 2),
            m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_ENLIGHTENED};
        m_weapons = {&m_tzeentchianSpear, &m_tzeentchianSpearAviarch, &m_viciousBeak, &m_teethAndHorns};
        m_hasMount = true;
        m_teethAndHorns.setMount(true);
    }

    bool TzaangorEnlightenedOnDisks::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        auto aviarch = new Model(g_basesize, wounds());
        aviarch->addMeleeWeapon(&m_tzeentchianSpearAviarch);
        aviarch->addMeleeWeapon(&m_viciousBeak);
        aviarch->addMeleeWeapon(&m_teethAndHorns);
        addModel(aviarch);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_tzeentchianSpear);
            model->addMeleeWeapon(&m_viciousBeak);
            model->addMeleeWeapon(&m_teethAndHorns);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *TzaangorEnlightenedOnDisks::Create(const ParameterList &parameters) {
        auto *unit = new TzaangorEnlightenedOnDisks();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TzaangorEnlightenedOnDisks::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TzaangorEnlightenedOnDisks::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TzaangorEnlightenedOnDisks::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH, BEASTS_OF_CHAOS}
            };
            s_registered = UnitFactory::Register("Tzaangor Enlightened on Disks", factoryMethod);
        }
    }

    Rerolls TzaangorEnlightenedOnDisks::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Guided by the Past
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        bool enemyHasFought = false;
        for (auto ip : units) {
            enemyHasFought |= ip->hasFought();
        }
        if (enemyHasFought) return Rerolls::Failed;

        return TzeentchBase::toHitRerolls(weapon, target);
    }

    Rerolls TzaangorEnlightenedOnDisks::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Guided by the Past
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        bool enemyHasFought = false;
        for (auto ip : units) {
            enemyHasFought |= ip->hasFought();
        }
        if (enemyHasFought) return Rerolls::Failed;

        return TzeentchBase::toWoundRerolls(weapon, target);
    }

    int TzaangorEnlightenedOnDisks::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch