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

    class Vargskyr : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Vargskyr();

        ~Vargskyr() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int rollChargeDistance() override;

    private:

        Weapon  m_talons{Weapon::Type::Melee, "Talons", 2, 4, 3, 3, -1, 2},
                m_maw{Weapon::Type::Melee, "Gaping Maw", 1, 1, 3, 2, -2, RAND_D3};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bounding Leaps                   Yes
// Gnarled Hide                     Yes
//

} // namespace Soulblight
