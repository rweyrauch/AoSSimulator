/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SORCEREROFNURGLE_H
#define SORCEREROFNURGLE_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class SorcererOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SorcererOfNurgle();
    ~SorcererOfNurgle() override = default;

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
// Stream of Corruption             TODO
//

} // SlavesToDarkness

#endif //SORCEREROFNURGLE_H