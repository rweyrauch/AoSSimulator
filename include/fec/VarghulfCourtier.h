/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class VarghulfCourtier : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        VarghulfCourtier();

        ~VarghulfCourtier() override = default;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_immenseClaws,
                m_daggerlikeFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Feed on Dark Magic               TODO
// King's Champion                  TODO
// Victory Feast                    TODO
// Muster Royal Household           Yes
//

} // namespace FleshEaterCourt
