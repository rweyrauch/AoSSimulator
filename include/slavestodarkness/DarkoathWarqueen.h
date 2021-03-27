/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class DarkoathWarqueen : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DarkoathWarqueen();

        ~DarkoathWarqueen() override = default;

    protected:

        void configure();

    private:

        Weapon m_axe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Infernal Runeshield              TODO
// Savage Duellist                  TODO
// The Will of the Gods             TODO
//

} // SlavesToDarkness
