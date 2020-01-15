/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSSORCERER_H
#define CHAOSSORCERER_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosSorcerer : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ChaosSorcerer();
    ~ChaosSorcerer() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_blade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 No
// Deamonic Power                   No
// Mark of Chaos                    No
//

} // SlavesToDarkness

#endif //CHAOSSORCERER_H