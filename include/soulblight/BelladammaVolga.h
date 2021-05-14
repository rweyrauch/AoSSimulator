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

    class BelladammaVolga : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BelladammaVolga(CursedBloodline bloodline, Lore lore, bool isGeneral);

        ~BelladammaVolga() override = default;

    private:

        Weapon  m_scimatar{Weapon::Type::Melee, "Timeworn Scimitar", 1, 3, 3, 3, -1, RAND_D3},
                m_fangsAndClaws{Weapon::Type::Melee, "Lupine Fangs and Claws", 1, 6, 4, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       TODO
// First of the Vyrkos              TODO
// Lycancurse                       TODO
// Under a Killing Moon             TODO
// Pack Alpha                       TODO
//

} // namespace Soulblight
