/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GUTROTSPUME_H
#define GUTROTSPUME_H

#include "nurgle/Nurgle.h"

namespace Nurgle {

    class GutrotSpume : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GutrotSpume();

        ~GutrotSpume() override = default;

        bool configure();

    protected:


    private:

        Weapon m_axe,
                m_tentacles;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Clutching Pseudopods             TODO
// Towering Arrogance               TODO
// Master of the Slime Feet         TODO
//

} // Nurgle

#endif //GUTROTSPUME_H
