/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLOPPITYBILEPIPER_H
#define SLOPPITYBILEPIPER_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class SloppityBilepiperHeraldOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SloppityBilepiperHeraldOfNurgle();
    ~SloppityBilepiperHeraldOfNurgle() override = default;

    bool configure();

protected:

private:

    Weapon m_marotter;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
//

} // Nurgle

#endif //SLOPPITYBILEPIPER_H