/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/FlamersOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 520;

    bool FlamersOfTzeentch::s_registered = false;

    FlamersOfTzeentch::FlamersOfTzeentch(ChangeCoven coven, int numModels, int points) :
            TzeentchBase(coven, "Flamers of Tzeentch", 9, g_wounds, 10, 5, true, points),
            m_warpflame(Weapon::Type::Missile, "Warpflame", 18, 3, 4, 3, 0, RAND_D3),
            m_warpflamePyrocaster(Weapon::Type::Missile, "Warpflame", 18, 4, 4, 3, 0, RAND_D3),
            m_flamingMaw(Weapon::Type::Melee, "Flaming Maw", 1, 2, 5, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, FLAMER, FLAMERS_OF_TZEENTCH};
        m_weapons = {&m_warpflame, &m_warpflamePyrocaster, &m_flamingMaw};

        auto pyrocaster = new Model(g_basesize, wounds());
        pyrocaster->addMissileWeapon(&m_warpflamePyrocaster);
        pyrocaster->addMeleeWeapon(&m_flamingMaw);
        addModel(pyrocaster);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_warpflame);
            model->addMeleeWeapon(&m_flamingMaw);
            addModel(model);
        }
    }

    Unit *FlamersOfTzeentch::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        auto numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new FlamersOfTzeentch(coven, numModels, ComputePoints(parameters));
    }

    void FlamersOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FlamersOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    FlamersOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Flamers of Tzeentch", factoryMethod);
        }
    }

    Wounds FlamersOfTzeentch::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = TzeentchBase::computeReturnedDamage(weapon, saveRoll);

        // Touched by Fire
        Dice::RollResult mortalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);
        wounds.mortal += mortalSaves.rollsGE(5);

        return wounds;
    }

    int FlamersOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = TzeentchBase::toHitModifier(weapon, target);

        // Capricious Warpflame
        if (target->remainingModels() >= 20) mod += 2;
        else if (target->remainingModels() >= 10) mod++;

        // Guided by Billowing Flames
        if (weapon->name() == m_warpflame.name()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
            for (auto ip : units) {
                if (ip->hasKeyword(EXALTED_FLAMERS)) {
                    mod++;
                    break;
                }
            }
        }
        return mod;
    }

    int FlamersOfTzeentch::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch