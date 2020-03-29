/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAUGHTERBRUTE_H
#define SLAUGHTERBRUTE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Slaughterbrute : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Slaughterbrute();

        ~Slaughterbrute() override = default;

        bool configure();

    protected:


    private:

        Weapon m_claws,
                m_jaws,
                m_talons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sigils of Enslavement            TODO
// Beast Unbound                    TODO
//

} // SlavesToDarkness

#endif //SLAUGHTERBRUTE_H
