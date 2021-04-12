/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusGuard : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusGuard(WayOfTheSeraphon way, Constellation constellation, int numModels, bool iconBearer, bool wardrum);

        SaurusGuard() = delete;

        ~SaurusGuard() override;

    protected:

        Rerolls chargeRerolls() const override;

        int stardrakeIcon(const Unit *target);

    private:

        Weapon  m_celestitePolearm{Weapon::Type::Melee, "Celestite Polearm", 1, 2, 3, 3, -1, 1},
                m_celestitePolearmAlpha{Weapon::Type::Melee, "Celestite Polearm", 1, 3, 3, 3, -1, 1},
                m_jaws{Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   Yes
// Wardrum                          Yes
// Selfless Protectors              TODO
//

} // namespace Seraphon
