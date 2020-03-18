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

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

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
// Disqustingly Resilient           TODO
// In Death There is Life           TODO
// Eruptive Infestation             TODO
//

} // Nurgle

#endif //POXBRINGER_H