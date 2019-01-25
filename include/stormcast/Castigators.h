/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CASTIGATORS_H
#define CASTIGATORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Castigators : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 18;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Castigators();
    ~Castigators() override = default;

    bool configure(int numModels);

protected:

private:

    static Weapon s_thunderheadGreatbow,
        s_thunderheadGreatbowPrime,
        s_heavyStock;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Burst of Celestial Energy        No
// Aetheric Channeling              No
//

} // namespace StormcastEternals

#endif //CASTIGATORS_H