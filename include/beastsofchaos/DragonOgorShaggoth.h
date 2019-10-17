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

    static const int BASESIZE = 90; // x52 oval;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DragonOgorShaggoth();
    ~DragonOgorShaggoth() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_stormWroughtAxe,
        m_sweepingTail,
        m_talonedForelimbs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Beneath the Tempest              No
// Summon Lightning                 No
//

} // namespace BeastsOfChaos

#endif //DRAGONOGORSHAGGOTH_H