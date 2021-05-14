/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class PrinceDuvalle : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PrinceDuvalle(Legion legion, Lore lore, bool isGeneral);

        ~PrinceDuvalle() override = default;

    private:

        Weapon  m_blade{Weapon::Type::Melee, "Possessed Blade", 1, 4, 3, 3, -1, 2};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace Soulblight
