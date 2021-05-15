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

    class GorslavTheGravekeeper : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GorslavTheGravekeeper(bool isGeneral);

        ~GorslavTheGravekeeper() override = default;

    private:

        Weapon  m_spade{Weapon::Type::Melee, "Gravekeeper's Spade", 2, 3, 4, 3, -1, RAND_D3};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Keeper of the Corpse-garden      TODO
// Arise! Arise!                    TODO
//

} // namespace Soulblight
