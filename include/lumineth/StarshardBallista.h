/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class VanariStarshardBallistas : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~VanariStarshardBallistas() override = default;

    protected:

        VanariStarshardBallistas(GreatNation nation);

    private:

        Weapon m_bolts{Weapon::Type::Missile, "Starshard Bolts", 30, 2, 3, 3, -2, RAND_D3},
            m_swords{Weapon::Type::Melee, "Arming Swords", 1, 2, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blinding Bolts                   TODO
// Messenger Hawk                   TODO
// Warding Lanterns                 TODO
//

} // namespace LuminethRealmLords

