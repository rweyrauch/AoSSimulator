/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <WarhammerSim.h>
#include <Unit.h>
#include <Board.h>
#include <stormcast/Liberators.h>
#include <khorne/Bloodreavers.h>
#include <ManoAMano.h>

TEST(ManoAMano, LiberatorsVsBloodreavers)
{
    ManoAMano battle(5, true);

    auto board = Board::Instance();
    board->setSize(72, 48);

    auto libs = new StormcastEternals::Liberators();
    auto reavers = new Khorne::Bloodreavers();

    bool ok = libs->configure(10, StormcastEternals::Liberators::Warhammer, 0, 0);
    ASSERT_TRUE(ok);

    ok = reavers->configure(30, Khorne::Bloodreavers::ReaverBlades, true, true);
    ASSERT_TRUE(ok);

    battle.combatants(libs, reavers);

    battle.start();

    while (!battle.done())
    {
        battle.simulate();

        battle.next();
    }

    auto victor = battle.getVictor();
    std::cout << "Team " << PlayerIdToString(victor) << " was victorious." << std::endl;

    delete libs;
    delete reavers;
}

TEST(ManoAMano, StatsLibsVsReavers)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    const int NUM_BATTLES = 1000;
    for (auto i = 0; i < NUM_BATTLES; i++)
    {
        ManoAMano battle(5, false);

        auto board = Board::Instance();
        board->setSize(72, 48);

        auto libs = new StormcastEternals::Liberators();
        auto reavers = new Khorne::Bloodreavers();

        bool ok = libs->configure(10, StormcastEternals::Liberators::Warhammer, 0, 0);
        ASSERT_TRUE(ok);

        ok = reavers->configure(30, Khorne::Bloodreavers::ReaverBlades, true, true);
        ASSERT_TRUE(ok);

        battle.combatants(libs, reavers);

        battle.start();

        while (!battle.done())
        {
            battle.simulate();
            battle.next();
        }

        auto victor = battle.getVictor();
        if (victor == PlayerId::Blue)
            blueVictories++;
        else if (victor == PlayerId::Red)
            redVictories++;
        else
            ties++;

        delete libs;
        delete reavers;
    }

    std::cout << "Victor Breakdown (%):" << std::endl
        << "\tRed: " << (float)redVictories * 100.0f/NUM_BATTLES << std::endl
        << "\tBlue: " << (float)blueVictories * 100.0f/NUM_BATTLES << std::endl
        << "\tTies: " << (float)ties * 100.0f/NUM_BATTLES << std::endl;
}