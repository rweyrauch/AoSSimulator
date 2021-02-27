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

    class ChaosSpawn : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        ChaosSpawn();

        ~ChaosSpawn() override = default;

        bool configure(int numModels);

    protected:

        void onFriendlyUnitSlain(const Unit *attacker) override;
        void onEnemyUnitSlain(const Unit* unit) override;

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
