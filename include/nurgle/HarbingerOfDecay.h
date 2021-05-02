/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "nurgle/Nurgle.h"

namespace Nurgle {

    class HarbingerOfDecay : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        HarbingerOfDecay();

        ~HarbingerOfDecay() override = default;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_scythe,
                m_bite;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Soulbound Shield                 Yes
// Rotsword                         TODO
// Morbid Vigour                    TODO
//

} // Nurgle

