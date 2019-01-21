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
#include <stormcast/Sequitors.h>
#include <khorne/Bloodreavers.h>
#include <gloomspitegitz/Stabbas.h>
#include <ManoAMano.h>

TEST(ManoAMano, LiberatorsVsBloodreavers)
{
    ManoAMano battle(5, true);

    auto board = Board::Instance();

    auto libs = new StormcastEternals::Liberators();
    auto reavers = new Khorne::Bloodreavers();

    bool ok = libs->configure(10, StormcastEternals::Liberators::Warhammer, false, 2, 0);
    ASSERT_TRUE(ok);

    ok = reavers->configure(30, Khorne::Bloodreavers::ReaverBlades, true, true);
    ASSERT_TRUE(ok);

    battle.combatants(libs, reavers);

    battle.start();

    board->render("LibsVsBlood_Start.png");

    while (!battle.done())
    {
        battle.simulate();

        auto round = battle.currentRound();
        std::stringstream str;
        str << "LibsVsBlook_Rnd_" << round << ".png";
        board->render(str.str());
        battle.next();
    }

    auto victor = battle.getVictor();
    std::cout << "Team " << PlayerIdToString(victor) << " was victorious." << std::endl;

    board->render("LibsVsBlood_End.png");

    delete libs;
    delete reavers;
}

TEST(ManoAMano, StatsLiberatorsVsReavers)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5, false);

    auto board = Board::Instance();

    auto libs = new StormcastEternals::Liberators();
    auto reavers = new Khorne::Bloodreavers();

    bool ok = libs->configure(10, StormcastEternals::Liberators::Warhammer, false, 2, 0);
    ASSERT_TRUE(ok);

    ok = reavers->configure(Khorne::Bloodreavers::MAX_UNIT_SIZE, Khorne::Bloodreavers::ReaverBlades, true, true);
    ASSERT_TRUE(ok);

    battle.combatants(libs, reavers);

    const int NUM_BATTLES = 10000;
    for (auto i = 0; i < NUM_BATTLES; i++)
    {
        libs->restore();
        reavers->restore();

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
    }

    std::cout << "Victor Breakdown (%):" << std::endl
        << "\tRed: " << (float)redVictories * 100.0f/NUM_BATTLES << std::endl
        << "\tBlue: " << (float)blueVictories * 100.0f/NUM_BATTLES << std::endl
        << "\tTies: " << (float)ties * 100.0f/NUM_BATTLES << std::endl;

    delete libs;
    delete reavers;
}


TEST(ManoAMano, StatsSequitersVsReavers)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5, false);

    auto seqs = new StormcastEternals::Sequitors();
    auto reavers = new Khorne::Bloodreavers();

    bool ok = seqs->configure(10, StormcastEternals::Sequitors::TempestBlade, 2, true, false);
    ASSERT_TRUE(ok);

    ok = reavers->configure(Khorne::Bloodreavers::MAX_UNIT_SIZE, Khorne::Bloodreavers::ReaverBlades, true, true);
    ASSERT_TRUE(ok);

    battle.combatants(seqs, reavers);

    const int NUM_BATTLES = 10000;
    for (auto i = 0; i < NUM_BATTLES; i++)
    {
        seqs->restore();
        reavers->restore();

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
    }

    std::cout << "Victor Breakdown (%):" << std::endl
              << "\tRed: " << (float)redVictories * 100.0f/NUM_BATTLES << std::endl
              << "\tBlue: " << (float)blueVictories * 100.0f/NUM_BATTLES << std::endl
              << "\tTies: " << (float)ties * 100.0f/NUM_BATTLES << std::endl;

    delete seqs;
    delete reavers;
}

TEST(ManoAMano, StatsLiberatorsVsStabbas)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5, false);

    auto libs = new StormcastEternals::Liberators();
    auto grots = new GloomspiteGitz::Stabbas();

    bool ok = libs->configure(StormcastEternals::Liberators::MAX_UNIT_SIZE, StormcastEternals::Liberators::Warhammer, false, 0, 0);
    ASSERT_TRUE(ok);

    ok = grots->configure(GloomspiteGitz::Stabbas::MAX_UNIT_SIZE, GloomspiteGitz::Stabbas::Stabba, GloomspiteGitz::Stabbas::Stabba, 0, 1, 1, 0);
    ASSERT_TRUE(ok);

    battle.combatants(libs, grots);

    const int NUM_BATTLES = 10000;
    for (auto i = 0; i < NUM_BATTLES; i++)
    {
        libs->restore();
        grots->restore();

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
    }

    std::cout << "Victor Breakdown (%):" << std::endl
              << "\tRed: " << (float)redVictories * 100.0f/NUM_BATTLES << std::endl
              << "\tBlue: " << (float)blueVictories * 100.0f/NUM_BATTLES << std::endl
              << "\tTies: " << (float)ties * 100.0f/NUM_BATTLES << std::endl;

    delete libs;
    delete grots;
}

TEST(ManoAMano, StatsSequitorVsStabbas)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5, false);

    auto seqs = new StormcastEternals::Sequitors();
    auto grots = new GloomspiteGitz::Stabbas();

    bool ok = seqs->configure(15, StormcastEternals::Sequitors::TempestBlade, 6, true, false);
    ASSERT_TRUE(ok);

    ok = grots->configure(GloomspiteGitz::Stabbas::MAX_UNIT_SIZE, GloomspiteGitz::Stabbas::Stabba, GloomspiteGitz::Stabbas::Stabba, 0, 1, 1, 0);
    ASSERT_TRUE(ok);

    battle.combatants(seqs, grots);

    const int NUM_BATTLES = 10000;
    for (auto i = 0; i < NUM_BATTLES; i++)
    {
        seqs->restore();
        grots->restore();

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
    }

    std::cout << "Victor Breakdown (%):" << std::endl
              << "\tRed: " << (float)redVictories * 100.0f/NUM_BATTLES << std::endl
              << "\tBlue: " << (float)blueVictories * 100.0f/NUM_BATTLES << std::endl
              << "\tTies: " << (float)ties * 100.0f/NUM_BATTLES << std::endl;

    delete seqs;
    delete grots;
}