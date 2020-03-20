/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSSPAWN_H
#define CHAOSSPAWN_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class ChaosSpawn : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        ChaosSpawn();

        ~ChaosSpawn() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_freakingMutations;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Curse of the Dark Gods           TODO
// Writhing Tentacles               TODO
//

} // namespace BeastsOfChaos

#endif //CHAOSSPAWN_H