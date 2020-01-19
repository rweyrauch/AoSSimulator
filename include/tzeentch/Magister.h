/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAGISTER_H
#define MAGISTER_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class Magister : public TzeentchBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Magister();
    ~Magister() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_sword;

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
