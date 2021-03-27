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

    class HorticulousSlimux : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        HorticulousSlimux();

        ~HorticulousSlimux() override;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Rerolls beastHandlerChargeReroll(const Unit *unit);

        Rerolls beastHandlerToHitRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_shears,
                m_jaws;

        lsignal::slot m_beastHandlerChargeSlot, m_beastHandlerToHitSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Acidic Slime Trail               TODO
// Beast Handler                    Yes
// In Death There is Life           TODO
// Cultivating the Garden of Nurgle TODO
//

} // Nurgle

