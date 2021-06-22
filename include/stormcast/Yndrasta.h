/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class Yndrasta : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Yndrasta(Stormhost stormhost, bool isGeneral);

        ~Yndrasta() override = default;

    protected:

    private:

        Weapon  m_thengavar{Weapon::Type::Missile, "Thengavar", 18, 1, 2, 2, -2, RAND_D6},
                m_blade{Weapon::Type::Melee, "Blade of the High Heavens", 1, 4, 3, 2, -2, 3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Prime Huntress               TODO
// Champion of Sigmar               TODO
// Dazzling Radiance                TODO
// Hawk of the Celestial Skies      TODO
//

} // namespace StormcastEternals
