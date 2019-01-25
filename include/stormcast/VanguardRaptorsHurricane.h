/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANGUARDRAPTORSHURRICANE_H
#define VANGUARDRAPTORSHURRICANE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VanguardRaptorsHurricane : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    VanguardRaptorsHurricane();
    ~VanguardRaptorsHurricane() override = default;

    bool configure(int numModels);

protected:

private:

    static Weapon s_hurricaneCrossbow,
        s_hurricaneCrossbowPrime,
        s_heavyStock;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rapid Fire                       No
// Suppressing Fire                 No
//

} // namespace StormcastEternals

#endif //VANGUARDRAPTORSHURRICANE_H