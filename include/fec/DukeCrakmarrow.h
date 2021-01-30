/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class DukeCrakmarrow : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DukeCrakmarrow();

        ~DukeCrakmarrow() override = default;

        bool configure();

    private:

        Weapon m_halberd;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gallant Champion                 TODO
// Muster the Grymwatch             TODO
//

} // namespace FleshEaterCourt
