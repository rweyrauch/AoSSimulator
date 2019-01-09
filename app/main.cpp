/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <cassert>
#include "Dice.h"
#include "Battle.h"

int main()
{
    Battle battle;

    Player player1, player2;

    battle.addPlayers(player1, player2);

    battle.start(PlayerId::Red);

    while (!battle.done())
    {
        battle.simulate();

        battle.next();
    }

    return 0;
}