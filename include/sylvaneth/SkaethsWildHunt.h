/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class SkaethsWildHunt : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkaethsWildHunt();

        ~SkaethsWildHunt() override = default;

    private:

        Weapon m_seekerBow{Weapon::Type::Missile, "Seeker Bow", 18, 1, 3, 4, -1, 1},
               m_javalin{Weapon::Type::Missile, "Javalin of the Hunt", 9, 1, 3, 3, -1, 2},
               m_javalinMelee{Weapon::Type::Melee, "Javalin of the Hunt (Melee)", 2, 1, 3, 3, -1, 2},
               m_huntingWeapon{Weapon::Type::Melee, "Hunting Weapon", 1, 2, 3, 4, 0, 1},
               m_teethAndClaws{Weapon::Type::Melee, "Teeth and Claws", 1, 2, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fleet of Foot                    Yes
// Might of Kurnoth                 TODO
//

} // namespace Sylvaneth
