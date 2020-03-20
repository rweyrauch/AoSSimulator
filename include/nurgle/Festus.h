/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FESTUS_H
#define FESTUS_H

#include "nurgle/Nurgle.h"

namespace Nurgle {

    class FestusTheLeechlord : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        FestusTheLeechlord();

        ~FestusTheLeechlord() override = default;

        bool configure();

    protected:


    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Healing Elixirs                  TODO
// Delightful Brews, Splendid Restoratives  TODO
// Curse of the Leper               TODO
//

} // Nurgle

#endif //FESTUS_H
