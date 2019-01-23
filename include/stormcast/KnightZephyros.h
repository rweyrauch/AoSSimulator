/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTZEPHYROS_H
#define KNIGHTZEPHYROS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightZephyros : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    KnightZephyros();
    ~KnightZephyros() override = default;

    bool configure();

protected:

private:

    static Weapon s_boltstormPistol,
        s_tempestAxes;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lightning-fast Strikes           No
// Tireless Hunter                  No
// Windrider                        No
//

} // namespace StormcastEternals

#endif //KNIGHTZEPHYROS_H