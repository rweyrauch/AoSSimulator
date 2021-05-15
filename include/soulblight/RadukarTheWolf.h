/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class RadukarTheWolf : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RadukarTheWolf(Lore lore, bool isGeneral);

        ~RadukarTheWolf() override = default;

    private:

        Weapon  m_blade{Weapon::Type::Melee, "Vyrkos Barrow-blade", 1, 4, 3, 3, -1, RAND_D3};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       TODO
// Supernatural Strength            TODO
// Loyal to the Last                TODO
// Call to the Hunt                 TODO
//

} // namespace Soulblight
