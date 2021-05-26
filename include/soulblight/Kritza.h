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

    class KritzaTheRatPrince : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit KritzaTheRatPrince(bool isGeneral);

        ~KritzaTheRatPrince() override = default;

    private:

        Weapon  m_blade{Weapon::Type::Melee, "Gnawblade", 1, 4, 3, 3, -1, RAND_D3};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scurrying Retreat                TODO
// Nauseating Aroma                 TODO
//

} // namespace Soulblight
