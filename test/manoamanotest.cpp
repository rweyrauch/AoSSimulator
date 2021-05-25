/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <AgeOfSigmarSim.h>
#include <Unit.h>
#include <Board.h>
#include <stormcast/Liberators.h>
#include <stormcast/Sequitors.h>
#include <khorne/Bloodreavers.h>
#include <gloomspitegitz/Stabbas.h>
#include <ManoAMano.h>

TEST(ManoAMano, LiberatorsVsBloodreavers)
{
    ManoAMano battle(5);

    auto board = Board::Instance();

    auto libs = std::make_shared<StormcastEternals::Liberators>(StormcastEternals::Stormhost::None, false, 10, StormcastEternals::Liberators::Warhammer, false, 2, 0, 0);
    auto reavers = std::make_shared<Khorne::Bloodreavers>(Khorne::SlaughterHost::None, 30, Khorne::Bloodreavers::Reaver_Blades, true, true, 0);
    reavers->formation(2);

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
}

TEST(ManoAMano, StatsLiberatorsVsReavers)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5);

    auto board = Board::Instance();

    auto libs = std::make_shared<StormcastEternals::Liberators>(StormcastEternals::Stormhost::None, false, 10, StormcastEternals::Liberators::Warhammer, false, 2, 0, 0);
    auto reavers = std::make_shared<Khorne::Bloodreavers>(Khorne::SlaughterHost::None, 20, Khorne::Bloodreavers::Reaver_Blades, true, true, 0);

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
}


TEST(ManoAMano, StatsSequitersVsReavers)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5);

    auto seqs = std::make_shared<StormcastEternals::Sequitors>(StormcastEternals::Stormhost::None, 10, StormcastEternals::Sequitors::Tempest_Blade, 2, true, false, 0);
    auto reavers = std::make_shared<Khorne::Bloodreavers>(Khorne::SlaughterHost::None, 20, Khorne::Bloodreavers::Reaver_Blades, true, true, 0);

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
}

TEST(ManoAMano, StatsLiberatorsVsStabbas)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5);

    auto libs = std::make_shared<StormcastEternals::Liberators>(StormcastEternals::Stormhost::None, false, 10, StormcastEternals::Liberators::Warhammer, false, 0, 0, 0);
    auto grots = std::make_shared<GloomspiteGitz::Stabbas>(60, GloomspiteGitz::Stabbas::Stabba, GloomspiteGitz::Stabbas::Stabba, 0, 1, 1, 0, 0);

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
}

TEST(ManoAMano, StatsSequitorVsStabbas)
{
    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    ManoAMano battle(5);

    auto seqs = std::make_shared<StormcastEternals::Sequitors>(StormcastEternals::Stormhost::None, 15, StormcastEternals::Sequitors::Tempest_Blade, 6, true, false, 0);
    auto grots = std::make_shared<GloomspiteGitz::Stabbas>(60, GloomspiteGitz::Stabbas::Stabba, GloomspiteGitz::Stabbas::Stabba, 0, 1, 1, 0, 0);

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
}