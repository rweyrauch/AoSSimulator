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

namespace StormcastEternals {

    class Retributors : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Retributors(Stormhost stormhost, int numModels, int numStarsoulMaces);

        ~Retributors() override = default;

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_lightningHammer,
                m_lightningHammerPrime,
                m_starsoulMace;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blast to Ashes                   Yes
// Starsoul Mace                    Yes
//

} // namespace StormcastEternals
