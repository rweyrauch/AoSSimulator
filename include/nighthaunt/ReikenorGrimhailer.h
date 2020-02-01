/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef REIKENORGRIMHAILER_H
#define REIKENORGRIMHAILER_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class ReikenorTheGrimhailer : public Nighthaunt
{
public:

    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 170;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    ReikenorTheGrimhailer();
    ~ReikenorTheGrimhailer() override = default;

    bool configure();

protected:

private:

    Weapon m_fellreaper,
        m_hoovesAndTeeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Corpse Candles                   No
// Frightful Touch                  No
// Reaped Like Corn                 No
// Wraithstorm                      No
//


} // namespace Nighthaunt

#endif // REIKENORGRIMHAILER_H
