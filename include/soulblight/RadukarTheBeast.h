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

    class RadukarTheBeast : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RadukarTheBeast(CursedBloodline bloodline, bool isGeneral);

        ~RadukarTheBeast() override = default;

    private:

        Weapon  m_claws{Weapon::Type::Melee, "Blood-slick Claws", 2, 6, 3, 3, -1, 2},
                m_blade{Weapon::Type::Melee, "Piercing Blade", 1, 6, 3, 3, -1, RAND_D3};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bounding Charge                  TODO
// The Hunger                       TODO
// Supernatural Reflexes            TODO
// Unleashed Ferocity               TODO
// Call to the Hunt                 TODO
// Mustering Howl                   TODO
//

} // namespace Soulblight
