/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace StormcastEternals {

    class SteelheartsChampions : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SteelheartsChampions();

        ~SteelheartsChampions() override = default;

    protected:

        void configure();

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_broadSword,
                m_grandhammer,
                m_warhammer;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Severin Steelheart               Yes
// Lay Low the Tyrants              Yes
// Heroic Guard                     TODO
// Sigmarite Shields                Yes
//

} // namespace StormcastEternals
