/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NURGLINGS_H
#define NURGLINGS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class Nurglings : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Nurglings();
    ~Nurglings() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_teeth;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disease-ridden Demise            TODO
// Endless Swarm                    TODO
// Hidden Infestation               TODO
//

} // Nurgle

#endif //NURGLINGS_H