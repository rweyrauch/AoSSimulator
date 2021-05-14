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

    class TheCrimsonCourt : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TheCrimsonCourt(Legion legion);

        ~TheCrimsonCourt() override = default;

    private:

        Weapon  m_mace{Weapon::Type::Melee, "Soulbound Mace", 2, 2, 3, 2, 0, RAND_D3},
                m_blades{Weapon::Type::Melee, "Paired Blades", 1, 3, 4, 3, -1, 1},
                m_bludgeon{Weapon::Type::Melee, "Honed Bludgeon", 1, 2, 4, 3, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace Soulblight
