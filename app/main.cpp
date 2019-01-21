/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <cassert>
#include <sstream>
#include <stormcast/Liberators.h>
#include <khorne/Bloodreavers.h>
#include <Board.h>
#include "Dice.h"
#include "ManoAMano.h"
#include "cxxopts.hpp"

void displayUnits(bool verbose);

int main(int argc, char* argv[])
{
    int numRounds = 5;
    bool verbose = false;
    bool listUnits = false;

    cxxopts::Options options(argv[0], " - Age of Sigmar: Mano a Mano simulation.");
    options.add_options()
        ("h, help", "Print help")
        ("l, list", "List supported units")
        ("r, rounds", "Number of battle rounds", cxxopts::value<int>(), "5")
        ("v, verbose", "Enable verbose logging")
        ;
    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }
    if (result.count("verbose"))
    {
        verbose = true;
    }
    if (result.count("list"))
    {
        listUnits = true;
    }

    Initialize();

    if (listUnits)
    {
        displayUnits(verbose);
        return EXIT_SUCCESS;
    }

    ManoAMano battle(numRounds, verbose);

    auto board = Board::Instance();

    auto libs = new StormcastEternals::Liberators();
    bool ok = libs->configure(10, StormcastEternals::Liberators::Warhammer, false, 2, 0);

    auto reavers = new Khorne::Bloodreavers();
    ok = reavers->configure(30, Khorne::Bloodreavers::ReaverBlades, true, true);
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

    delete libs;
    delete reavers;

    return EXIT_SUCCESS;
}

void displayUnits(bool verbose)
{
    std::cout << "Supported Units: " << std::endl;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        std::cout << "\t" << ruip->first << std::endl;

        if (verbose)
        {
            if (ruip->second.m_parameters.empty())
            {
                std::cout << "\t  Parameters:  None" << std::endl;
                continue;
            }

            std::cout << "\t  Parameters:" << std::endl;
            for (auto pip : ruip->second.m_parameters)
            {
                if (pip.m_paramType == ParamType::Integer)
                {
                    if (ruip->second.m_paramToString == nullptr)
                    {
                        if (pip.m_increment > 0)
                        {
                            std::cout << "\t\tName: " << pip.m_name << " Type: Integer  Value: "
                                      << pip.m_intValue << "  Allowed Values: ";
                            for (auto v = pip.m_minValue; v <= pip.m_maxValue; v += pip.m_increment)
                            {
                                std::cout << v;
                                if (v < pip.m_maxValue) std::cout << ", ";
                            }
                            std::cout << std::endl;
                        }
                        else
                        {
                            std::cout << "\t\tName: " << pip.m_name << " Type: Integer  Value: "
                                      << pip.m_intValue
                                      << "  Min: " << pip.m_minValue << "  Max: " << pip.m_maxValue
                                      << std::endl;
                        }
                    }
                    else if (pip.m_increment > 0)
                    {
                        std::cout << "\t\tName: " << pip.m_name << " Type: Integer  Value: "
                                  << ruip->second.m_paramToString(pip) << "  Allowed Values: ";
                        Parameter parm(pip);
                        for (auto v = pip.m_minValue; v <= pip.m_maxValue; v += pip.m_increment)
                        {
                            parm.m_intValue = v;
                            std::cout << ruip->second.m_paramToString(parm);
                            if (v < pip.m_maxValue) std::cout << ", ";
                        }
                        std::cout << std::endl;
                    }
                    else
                    {
                        std::cout << "\t\tName: " << pip.m_name << " Type: Integer  Value: "
                                  << ruip->second.m_paramToString(pip) << "  Min: " << pip.m_minValue
                                  << "  Max: " << pip.m_maxValue << std::endl;
                    }
                }
                else if (pip.m_paramType == ParamType::Boolean)
                {
                    std::cout << "\t\tName: " << pip.m_name << " Type: Boolean  Value: "
                              << (pip.m_boolValue ? "true" : "false") << std::endl;
                }
            }
        }
    }
}
