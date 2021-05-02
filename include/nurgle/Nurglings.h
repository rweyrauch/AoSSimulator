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

    class Nurglings : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Nurglings(int points);

        ~Nurglings() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_teeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disease-ridden Demise            TODO
// Endless Swarm                    TODO
// Hidden Infestation               TODO
//

} // Nurgle
