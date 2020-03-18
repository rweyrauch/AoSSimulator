/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORGHOTTS_H
#define ORGHOTTS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class OrghottsDaemonspew : public NurgleBase
{
public:

    static const int BASESIZE = 130;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 0;


    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    OrghottsDaemonspew();
    ~OrghottsDaemonspew() override = default;

    bool configure();

protected:

private:

    Weapon m_tongue,
        m_rotaxes,
        m_claws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
//

} // Nurgle

#endif //ORGHOTTS_H