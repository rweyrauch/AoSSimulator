/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EXALTEDHERO_H
#define EXALTEDHERO_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ExaltedHeroOfChaos : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ExaltedHeroOfChaos();
    ~ExaltedHeroOfChaos() override = default;

    bool configure();

protected:

private:

    Weapon m_blades;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    No
// Dark Blessings                   No
// Glory-hungry Bladesman           No
// Thrice-damned Dagger             No
// Trail of Red Ruin                No
//

} // SlavesToDarkness

#endif //EXALTEDHERO_H