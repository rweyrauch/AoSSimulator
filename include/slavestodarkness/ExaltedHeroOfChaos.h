/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EXALTEDHERO_H
#define EXALTEDHERO_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ExaltedHeroOfChaos : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
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
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Dark Blessings                   TODO
// Glory-hungry Bladesman           TODO
// Thrice-damned Dagger             TODO
// Trail of Red Ruin                TODO
//

} // SlavesToDarkness

#endif //EXALTEDHERO_H