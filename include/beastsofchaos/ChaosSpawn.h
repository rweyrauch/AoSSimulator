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

        static int ComputePoints(const ParameterList& parameters);

        ChaosSpawn() = delete;

        ~ChaosSpawn() override = default;

        ChaosSpawn(Greatfray fray, int numModels, int points);

    protected:

        void onFriendlyUnitSlain(const Unit *attacker) override;
        void onEnemyUnitSlain(const Unit* unit) override;

    private:

        Weapon m_freakingMutations{Weapon::Type::Melee, "Freakish Mutations", 1, RAND_2D6, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Curse of the Dark Gods           TODO
// Writhing Tentacles               TODO
//

} // namespace BeastsOfChaos
