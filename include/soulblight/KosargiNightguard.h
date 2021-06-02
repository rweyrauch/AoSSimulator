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

        explicit KosargiNightguard();

        ~KosargiNightguard() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_bardiche{Weapon::Type::Melee, "Bardiche", 2, 2, 3, 3, -1, 2};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathly Vigour                   Yes
// Servants Even in Death           Yes
//

} // namespace Soulblight
