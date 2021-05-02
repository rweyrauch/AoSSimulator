/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Salamanders : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Salamanders(WayOfTheSeraphon way, Constellation constellation, int numModels, int points);

        Salamanders() = delete;

        ~Salamanders() override = default;

    private:

        Weapon  m_streamOfFire{Weapon::Type::Missile, "Stream of Fire", 12, 4, 3, 3, -2, RAND_D3},
                m_jaws{Weapon::Type::Melee, "Burning Jaws", 1, 3, 3, 3, -2, RAND_D3},
                m_goad{Weapon::Type::Melee, "Celestite Goad", 1, 1, 4, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// It Burns!                        TODO
//

} // namespace Seraphon
