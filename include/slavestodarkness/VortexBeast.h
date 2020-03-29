/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VORTEXBEAST_H
#define VORTEXBEAST_H


#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class MutalithVortexBeast : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MutalithVortexBeast();

        ~MutalithVortexBeast() override = default;

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
// Mutant Regeneration              TODO
// Aura of Mutation                 TODO
//

} // SlavesToDarkness

#endif //VORTEXBEAST_H
