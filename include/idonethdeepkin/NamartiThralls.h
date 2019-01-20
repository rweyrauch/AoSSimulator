/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NAMARTITHRALLS_H
#define NAMARTITHRALLS_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class NamartiThralls : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    NamartiThralls();
    ~NamartiThralls() override = default;

    bool configure(int numModels, int numIconBearers);

protected:


private:

    static Weapon s_lanmariBlade;

    static bool s_registered;
};

} // namespace IdonethDeepkin

#endif // NAMARTITHRALLS_H
