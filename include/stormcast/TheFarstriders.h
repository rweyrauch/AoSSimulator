/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEFARSTRIDERS_H
#define THEFARSTRIDERS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace StormcastEternals
{

class TheFarstriders : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    TheFarstriders();
    ~TheFarstriders() override = default;

    bool configure();

protected:


private:

    Weapon m_boltstormPistol,
        m_shockHandaxe,
        m_stormSaber;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Astral Compass                   No
// Star Falcon                      No
// Tireless Hunters                 No
//

} // namespace StormcastEternals

#endif //THEFARSTRIDERS_H