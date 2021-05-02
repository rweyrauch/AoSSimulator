/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Slambo : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Slambo(DamnedLegion legion, bool isGeneral);

        Slambo() = delete;

        ~Slambo() override = default;

    private:

        Weapon  m_hurledAxe{Weapon::Type::Missile, "Hurled Chaos Axe", 8, 1, 3, 3, -1, RAND_D3},
                m_chaosAxes{Weapon::Type::Melee, "Chaos Axes", 1, RAND_D6, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Legendary Killer                 TODO
// Glory-seeking Axeman             TODO
//

} // SlavesToDarkness
