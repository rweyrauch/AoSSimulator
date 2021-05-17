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

    class TheSepulchralGuard : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TheSepulchralGuard(CursedBloodline bloodline);

        ~TheSepulchralGuard() override = default;

    private:

        Weapon  m_spear{Weapon::Type::Melee, "Ancient Spear", 2, 3, 4, 4, 0, 1},
                m_mace{Weapon::Type::Melee, "Ancient Mace", 1, 2, 4, 3, 0, 1},
                m_greatblade{Weapon::Type::Melee, "Ancient Greatblade", 1, 2, 4, 4, -1, 1},
                m_scythe{Weapon::Type::Melee, "Ancient Scythe", 1, 1, 4, 3, 0, 2},
                m_blade{Weapon::Type::Melee, "Ancient Blade", 1, 1, 4, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightening Speed                TODO
// Serve In Death                   TODO
//

} // namespace Soulblight
