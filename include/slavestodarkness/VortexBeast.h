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

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_claws,
                m_maw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mutant Regeneration              Yes
// Aura of Mutation                 Yes
//    Hideous Disfigurement         Yes
//    Troggbrains                   Yes
//    Gift of Mutations             Yes
//    Tide of Transmogrification    Yes
//    Maelstrom of Change           Yes
//    Spawnchange                   Yes
//

} // SlavesToDarkness

#endif //VORTEXBEAST_H
