/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class SpoilpoxScrivenerHeraldOfNurgle : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SpoilpoxScrivenerHeraldOfNurgle();

        ~SpoilpoxScrivenerHeraldOfNurgle() override;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_sneeze,
                m_maw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Keep Counting, I'm Watching You
//    Tally of Blows                TODO
//    Studied Lacerations           TODO
//    Recorded Stamina              TODO
//

} // Nurgle
