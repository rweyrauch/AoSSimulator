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

    class Castigators : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Castigators(Stormhost stormhost, int numModels);

        ~Castigators() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        void onStartShooting(PlayerId player) override;

    private:

        bool m_aethericChannellingPower = false;

        Weapon m_thunderheadGreatbow,
                m_thunderheadGreatbowPrime,
                m_heavyStock;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Burst of Celestial Energy        Yes
// Aetheric Channeling              Yes
//

} // namespace StormcastEternals
