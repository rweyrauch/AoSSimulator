/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_PLAYER_H
#define WARHAMMERSIM_PLAYER_H

#include "Roster.h"

class Player
{
public:
    Player() = default;

private:

    Roster m_roster;
};


#endif //WARHAMMERSIM_PLAYER_H
