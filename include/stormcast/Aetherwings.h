/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AETHERWINGS_H
#define AETHERWINGS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Aetherwings : public StormcastEternal
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 50;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    Aetherwings();
    ~Aetherwings() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_beakAndClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Watchful Guardians               No
// Swooping Hunters                 Yes
//

} // namespace StormcastEternals

#endif //AETHERWINGS_H