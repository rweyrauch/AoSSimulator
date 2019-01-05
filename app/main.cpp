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

    battle.start();

    while (!battle.done())
    {
        battle.run();

        battle.next();
    }

    return 0;
}