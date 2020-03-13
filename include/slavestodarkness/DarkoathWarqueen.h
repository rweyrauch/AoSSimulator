/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DARKOATHWARQUEEN_H
#define DARKOATHWARQUEEN_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class DarkoathWarqueen : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    DarkoathWarqueen();
    ~DarkoathWarqueen() override = default;

    bool configure();

protected:

private:

    Weapon m_axe;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Infernal Runeshield              TODO
// Savage Duellist                  TODO
// The Will of the Gods             TODO
//

} // SlavesToDarkness

#endif //DARKOATHWARQUEEN_H