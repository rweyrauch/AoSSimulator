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

namespace Nurgle
{

class FestusTheLeechlord : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
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
