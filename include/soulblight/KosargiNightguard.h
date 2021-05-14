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

    class KosargiNightguard : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KosargiNightguard(CursedBloodline bloodline, int numModels);

        ~KosargiNightguard() override = default;

    private:

        Weapon  m_bardiche{Weapon::Type::Melee, "Bardiche", 2, 2, 3, 3, -1, 2};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathly Vigour                   TODO
// Servants Even in Death           TODO
//

} // namespace Soulblight
