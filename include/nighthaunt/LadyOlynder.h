/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LADYOLYNDER_H
#define LADYOLYNDER_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class LadyOlynder : public Nighthaunt
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    LadyOlynder();
    ~LadyOlynder() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  No
// Grave-sands of Time              No
// Lifting the Veil                 No
// Mortarch of Grief                No
// Wail of the Damned               No
// No Rest For the Wicked           No
//


} // namespace Nighthaunt

#endif // LADYOLYNDER_H
