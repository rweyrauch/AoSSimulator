/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TIDECASTER_H
#define TIDECASTER_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class IsharannTidecaster : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    IsharannTidecaster();
    ~IsharannTidecaster() override = default;

    bool configure();

protected:


private:

    Weapon m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Spirit Guardians                 No
// The Wide Ethersea                No
// Riptide                          No
//

} // namespace IdonethDeepkin

#endif // TIDECASTER_H
