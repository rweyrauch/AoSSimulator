/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FURIES_H
#define FURIES_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class Furies : public SlavesToDarknessBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Furies();
    ~Furies() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_daggerAndClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Sneaky Little Devils             TODO
//

} // SlavesToDarkness

#endif //FURIES_H