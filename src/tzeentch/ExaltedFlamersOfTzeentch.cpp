/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/ExaltedFlamersOfTzeentch.h>
#include <UnitFactory.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 75; // x42
    static const int g_wounds = 4;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 600;

    bool ExaltedFlamersOfTzeentch::s_registered = false;

    ExaltedFlamersOfTzeentch::ExaltedFlamersOfTzeentch(ChangeCoven coven, int numModels, int points) :
            TzeentchBase(coven, "Exalted Flamers of Tzeentch", 9, g_wounds, 10, 5, true, points),
            m_warpflame(Weapon::Type::Missile, "Billowing Warpflame", 18, 6, 4, 3, -1, RAND_D3),
            m_flamingMaw(Weapon::Type::Melee, "Flaming Maw", 2, 4, 5, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, FLAMER, EXALTED_FLAMERS};
        m_weapons = {&m_warpflame, &m_flamingMaw};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_warpflame);
            model->addMeleeWeapon(&m_flamingMaw);
            addModel(model);
        }
    }

    Unit *ExaltedFlamersOfTzeentch::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new ExaltedFlamersOfTzeentch(coven, numModels, ComputePoints(parameters));
    }

    void ExaltedFlamersOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ExaltedFlamersOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    ExaltedFlamersOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Exalted Flamers of Tzeentch", factoryMethod);
        }
    }

    int ExaltedFlamersOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = TzeentchBase::toHitModifier(weapon, target);

        // Capricious Warpflame
        if (target->remainingModels() >= 20) mod += 2;
        else if (target->remainingModels() >= 10) mod++;

        return mod;
    }

    Wounds ExaltedFlamersOfTzeentch::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = TzeentchBase::computeReturnedDamage(weapon, saveRoll);

        // Touched by Fire
        Dice::RollResult mortalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);
        wounds.mortal += mortalSaves.rollsGE(5);

        return wounds;
    }

    int ExaltedFlamersOfTzeentch::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch