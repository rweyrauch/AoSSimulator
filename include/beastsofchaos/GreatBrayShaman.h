/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GREATBRAYSHAMAN_H
#define GREATBRAYSHAMAN_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class GreatBrayShaman : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }

    GreatBrayShaman();
    ~GreatBrayShaman() override = default;

    bool configure();

protected:

private:

    Weapon m_fetishStaff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Infuse with Bestial Vigour       No
// Devolve                          No
//

} // namespace BeastsOfChaos

#endif //GREATBRAYSHAMAN_H