/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAGISTERONDISC_H
#define MAGISTERONDISC_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class MagisterOnDiscOfTzeentch : public TzeentchBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    MagisterOnDiscOfTzeentch();
    ~MagisterOnDiscOfTzeentch() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_sword,
        m_teethAndHorns;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Magic-touched                    No
// Bolt of Change                   No
//

} // namespace Tzeentch

#endif //MAGISTERONDISC_H
