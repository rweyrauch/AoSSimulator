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

    class PoxbringerHeraldOfNurgle : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PoxbringerHeraldOfNurgle();

        ~PoxbringerHeraldOfNurgle() override = default;

    protected:

        void configure(Lore lore);

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_balesword;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disqustingly Resilient           Yes
// In Death There is Life           TODO
// Eruptive Infestation             TODO
//

} // Nurgle
