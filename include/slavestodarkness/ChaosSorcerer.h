/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSSORCERER_H
#define CHAOSSORCERER_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosSorcerer : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosSorcerer();

        ~ChaosSorcerer() override = default;

        bool configure();

    protected:

    private:

        Weapon m_staff,
                m_blade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 TODO
// Deamonic Power                   TODO
// Mark of Chaos                    Yes
//

} // SlavesToDarkness

#endif //CHAOSSORCERER_H