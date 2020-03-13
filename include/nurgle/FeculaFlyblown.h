/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FECULAFLYBLOWN_H
#define FECULAFLYBLOWN_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class FeculaFlyblown : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
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

#endif //FECULAFLYBLOWN_H
