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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Magic-touched                    TODO
// Bolt of Change                   TODO
//

} // namespace Tzeentch

#endif //MAGISTERONDISC_H
