/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPOILPOX_H
#define SPOILPOX_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class SpoilpoxScrivenerHeraldOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SpoilpoxScrivenerHeraldOfNurgle();
    ~SpoilpoxScrivenerHeraldOfNurgle() override = default;

    bool configure();

protected:


private:

    Weapon m_sneeze,
        m_maw;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
//

} // Nurgle

#endif //SPOILPOX_H