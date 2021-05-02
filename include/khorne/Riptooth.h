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

    class Riptooth : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ~Riptooth() override = default;

    protected:

        Riptooth();

        // Unflagging Hunter
        Rerolls chargeRerolls() const override { return Rerolls::Failed; }

    private:

        Weapon m_claws{Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Collar of Khorne                 Yes
// Unflagging Hunter                Yes
//

} // namespace Khorne
