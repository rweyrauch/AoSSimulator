/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAMBO_H
#define SLAMBO_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class Slambo : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Slambo();
    ~Slambo() override = default;

    bool configure();

protected:

private:

    Weapon m_hurledAxe,
        m_chaosAxes;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Legendary Killer                 No
// Glory-seeking Axeman             No
//

} // SlavesToDarkness

#endif //SLAMBO_H