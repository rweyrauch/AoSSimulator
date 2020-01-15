/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTVEXILLOR_H
#define KNIGHTVEXILLOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightVexillor : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels)  { return POINTS_PER_UNIT; }

    KnightVexillor();
    ~KnightVexillor() override = default;

    bool configure();

protected:

private:

    Weapon m_warhammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon of War                      No
// Meteoric Standard                No
// Pennant of the Stormbringer      No
//

} // namespace StormcastEternals

#endif //KNIGHTVEXILLOR_H