/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Battle.h"

void Battle::start()
{
    //
}

void Battle::next()
{
}

bool Battle::done()
{
    return false;
}

void Battle::run()
{

}

void Battle::players(const Player &player1, const Player &player2)
{
    m_players[0] = player1;
    m_players[1] = player2;
}
