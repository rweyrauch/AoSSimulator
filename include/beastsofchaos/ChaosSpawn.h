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

namespace BeastsOfChaos
{

class ChaosSpawn : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 50;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Curse of the Dark Gods           No
// Writhing Tentacles               No
//

} // namespace BeastsOfChaos

#endif //CHAOSSPAWN_H