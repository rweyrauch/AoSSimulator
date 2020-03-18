/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef POXBRINGER_H
#define POXBRINGER_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class PoxbringerHeraldOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    PoxbringerHeraldOfNurgle();
    ~PoxbringerHeraldOfNurgle() override = default;

    bool configure();

protected:


private:

    Weapon m_balesword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
//

} // Nurgle

#endif //POXBRINGER_H