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
    Dice dice;

    for (auto i = 0; i < 1000; i++)
    {
        auto r = dice.rollD6();
        assert(r >= 1);
        assert(r <= 6);

        r = dice.rollD3();
        assert(r >= 1);
        assert(r <= 3);

        r = dice.roll2D6();
        assert(r >= 2);
        assert(r <= 12);

        r = dice.roll3D6();
        assert(r >= 3);
        assert(r <= 18);

        auto rv = dice.rollD6(16);
        assert(rv.size() == 16);
    }

    Battle battle;

    battle.start();

    while (!battle.done())
    {
        battle.run();

        battle.next();
    }

    return 0;
}