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

    class LyriorUthralle : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~LyriorUthralle() override = default;

    protected:

        LyriorUthralle(bool isGeneral);

    private:

        Weapon m_daemonbane{Weapon::Type::Missile, "Daemonbane", 18, 1, 2, 2, -2, RAND_D3},
            m_daemonbaneMelee{Weapon::Type::Melee, "Daemonbane", 3, 1, 2, 2, -2, RAND_D3},
            m_sword{Weapon::Type::Melee, "Regent's Sword", 1, 5, 2, 3, -1, 1},
            m_hornsAndClaws{Weapon::Type::Melee, "Horns and Claws", 1, 4, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Daemonbane                       TODO
// Purest Aetherquartz              TODO
// Sunmetal Weapons                 TODO
// Voice of Tyrion                  TODO
// Greater Power of Hysh            TODO
//

} // namespace LuminethRealmLords

