/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class FleshHounds : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FleshHounds() = delete;

        ~FleshHounds() override = default;

    protected:

        FleshHounds(SlaughterHost host, int numModels, int points);

        // Unflagging Hunter
        Rerolls chargeRerolls() const override { return Rerolls::Failed; }

        int unbindingModifier() const override;

    private:


        Weapon  m_burningRoar{Weapon::Type::Missile, "Burning Roar", 8, 1, 2, 4, 0, 1},
                m_blooddarkClaws{Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Collars of Khorne                Yes
// Unflagging Hunters               Yes
//

} // namespace Khorne
