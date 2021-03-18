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

    class FeculaFlyblown : public NurgleBase {
    public:
        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        FeculaFlyblown();

        ~FeculaFlyblown() override = default;

        bool configure();

    protected:


    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blessed with Vitality            TODO
// Retchling                        TODO
// Stream of Corruption             TODO
//

} // Nurgle

