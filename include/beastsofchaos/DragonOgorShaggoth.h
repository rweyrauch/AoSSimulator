/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class DragonOgorShaggoth : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        DragonOgorShaggoth() = delete;

        ~DragonOgorShaggoth() override = default;

        DragonOgorShaggoth(Greatfray fray, Lore lore, CommandTrait trait, Artefact artefact, bool general);

    protected:

    private:

        Weapon m_stormWroughtAxe,
                m_sweepingTail,
                m_talonedForelimbs;

        Lore m_lore = Lore::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Beneath the Tempest              TODO
// Summon Lightning                 Yes
//

} // namespace BeastsOfChaos
