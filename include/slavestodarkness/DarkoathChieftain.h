/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DARKOATHCHIEFTAIN_H
#define DARKOATHCHIEFTAIN_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class DarkoathChieftain : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    DarkoathChieftain();
    ~DarkoathChieftain() override = default;

    bool configure();

protected:

private:

    Weapon m_axe,
        m_broadsword;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Berserk Charge                   No
// Deathblow                        No
// Last Gasp of Glory               No
//

} // SlavesToDarkness

#endif //DARKOATHCHIEFTAIN_H