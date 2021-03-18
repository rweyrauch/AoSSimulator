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

        static int ComputePoints(int numModels);

        static void Init();

        Slambo();

        ~Slambo() override = default;

        bool configure();

    protected:

    private:

        Weapon m_hurledAxe,
                m_chaosAxes;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Legendary Killer                 TODO
// Glory-seeking Axeman             TODO
//

} // SlavesToDarkness
