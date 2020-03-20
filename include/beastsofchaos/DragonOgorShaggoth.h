/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRAGONOGORSHAGGOTH_H
#define DRAGONOGORSHAGGOTH_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class DragonOgorShaggoth : public BeastsOfChaosBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    DragonOgorShaggoth();
    ~DragonOgorShaggoth() override = default;

    bool configure();

protected:

private:

    Weapon m_stormWroughtAxe,
        m_sweepingTail,
        m_talonedForelimbs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Beneath the Tempest              TODO
// Summon Lightning                 TODO
//

} // namespace BeastsOfChaos

#endif //DRAGONOGORSHAGGOTH_H