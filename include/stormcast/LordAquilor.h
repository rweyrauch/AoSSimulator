/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LordAquilor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        LordAquilor();

        ~LordAquilor() override = default;

    protected:

        void configure(bool astralCompass);

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        bool m_astralCompass = false;

        Weapon m_boltstormPistol,
                m_starboundBlade,
                m_shockHandaxe,
                m_beakAndClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aethereal Strike                 Yes
// Ride the Winds Aetheric          TODO
// Astral Compass                   TODO
// Lord of the Azyrite Hurricane    TODO

} // namespace StormcastEternals
