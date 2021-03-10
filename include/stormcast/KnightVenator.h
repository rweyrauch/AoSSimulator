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

    class KnightVenator : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        KnightVenator();

        ~KnightVenator() override = default;

        bool configure();

    protected:

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onRestore() override {
            StormcastEternal::onRestore();
            m_usedStarFatedArrow = false;
        }

        void onStartShooting(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        bool m_usedStarFatedArrow = false;

        Weapon m_realmhuntersBow,
                m_starFatedArrow,
                m_beakAndTalonsMissile,
                m_bowStave,
                m_beakAndTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Celestial Strike                 Yes
// Star-fated Arrow                 Yes
//

} // namespace StormcastEternals
