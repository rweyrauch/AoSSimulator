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

void displayUnits(bool verbose, const std::string& faction);
Unit* parseUnitDescription(const std::string& desc);

int main(int argc, char* argv[])
{
    int numRounds = 5;
    bool verbose = false;
    bool listUnits = false;
    std::string listFaction("all");
    int numIterations = 1;
    bool saveMaps = false;
    std::string mapBaseName("battlemap");

    cxxopts::Options options(argv[0], "Age of Sigmar: Mano a Mano simulation.");
    options.add_options()
        ("h, help", "Print help")
        ("l, list", "List supported units")
        ("f, faction", "List units the given faction", cxxopts::value<std::string>(), "all")
        ("r, rounds", "Number of battle rounds", cxxopts::value<int>(numRounds))
        ("v, verbose", "Enable verbose logging")
        ("1, red", "Player 1 (Red) Unit", cxxopts::value<std::string>(), "")
        ("2, blue", "Player 2 (Blue) Unit", cxxopts::value<std::string>(), "")
        ("s, save", "Save battlemaps")
        ("mapname", "Battlemap basename", cxxopts::value<std::string>(mapBaseName))
        ("i, iterations", "Number of battle iterations", cxxopts::value<int>(numIterations))
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
    if (result.count("faction"))
    {
        listFaction = result["faction"].as<std::string>();
    }
    if (result.count("save"))
    {
        saveMaps = true;
    }

    Initialize();

    if (listUnits)
    {
        displayUnits(verbose, listFaction);
        return EXIT_SUCCESS;
    }

    ManoAMano battle(numRounds, verbose);

    auto board = Board::Instance();

    if (result.count("red") == 0)
    {
        std::cout << "Must define a unit for player 1 (red)." << std::endl;
        return EXIT_FAILURE;
    }
    if (result.count("blue") == 0)
    {
        std::cout << "Must define a unit for player 2 (blue)." << std::endl;
        return EXIT_FAILURE;
    }

    // parse red string
    std::string redUnit = result["red"].as<std::string>();
    Unit* pRed = parseUnitDescription(redUnit);
    if (pRed == nullptr)
    {
        std::cout << "Failed to parse player 1 (red) unit description." << std::endl;
        return EXIT_FAILURE;
    }

    // TODO: parse blue string
    std::string blueUnit = result["blue"].as<std::string>();
    Unit* pBlue = parseUnitDescription(blueUnit);
    if (pBlue == nullptr)
    {
        std::cout << "Failed to parse player 2 (blue) unit description." << std::endl;
        return EXIT_FAILURE;
    }

    battle.combatants(pRed, pBlue);

    std::stringstream fn;

    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    for (auto i = 0; i < numIterations; i++)
    {
        pRed->restore();
        pBlue->restore();

        battle.start();

        if (saveMaps)
        {
            fn.str("");
            fn << mapBaseName << "_start.png";
            board->render(fn.str());
        }

        while (!battle.done())
        {
            battle.simulate();

            auto round = battle.currentRound();

            if (saveMaps)
            {
                fn.str("");
                fn << mapBaseName << "_round_" << round << ".png";
                board->render(fn.str());
            }

            battle.next();
        }

        auto victor = battle.getVictor();

        if (verbose)
            std::cout << "Team " << PlayerIdToString(victor) << " was victorious." << std::endl;

        if (victor == PlayerId::Blue)
            blueVictories++;
        else if (victor == PlayerId::Red)
            redVictories++;
        else
            ties++;

        if (saveMaps)
        {
            fn.str("");
            fn << mapBaseName << "_end.png";
            board->render(fn.str());
        }
    }

    std::cout << "Victor Breakdown (%):" << std::endl
              << "\tRed: " << (float)redVictories * 100.0f/numIterations << std::endl
              << "\tBlue: " << (float)blueVictories * 100.0f/numIterations << std::endl
              << "\tTies: " << (float)ties * 100.0f/numIterations << std::endl;

    delete pRed;
    delete pBlue;

    return EXIT_SUCCESS;
}

static std::map<std::string, int> g_factionNameLookup = {
    { "Stormcast Eternal", STORMCAST_ETERNAL, },
    { "Khorne", KHORNE },
    { "Sylvaneth", SYLVANETH },
    { "Gloomspite Gitz", GLOOMSPITE_GITZ },
    { "Moonclan", MOONCLAN },
    { "Nighthaunt", NIGHTHAUNT },
    { "Daughters of Khaine", DAUGHTERS_OF_KHAINE },
    { "Idoneth Deepkin", IDONETH_DEEPKIN }
};

void displayUnits(bool verbose, const std::string& faction)
{
    bool listAll = (faction == "all");

    std::cout << "Supported Units in Faction(" << faction << "):" << std::endl;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        if (!listAll)
        {
            auto ki = g_factionNameLookup.find(faction);
            if (ki != g_factionNameLookup.end())
            {
                // filter based on keyword
                auto unit = UnitFactory::Create(ruip->first, ruip->second.m_parameters);
                if (unit)
                {
                    auto keyword = (Keyword) ki->second;
                    const bool haveKeyword = unit->hasKeyword(keyword);
                    delete unit;
                    if (!haveKeyword)
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
        }

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

std::vector<std::string> split(const std::string& str, char delim)
{
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }
    return result;
}

Unit* parseUnitDescription(const std::string& desc)
{
    if (desc.empty())
        return nullptr;

    // <unit name>,<param1=value>,<param2=value>,<param3=value>,etc...
    auto args = split(desc, ',');
    if (args.empty())
        return nullptr;

    Unit* unit = nullptr;

    std::string unitName = *args.begin();
    auto factory = UnitFactory::LookupUnit(unitName);
    if (factory)
    {
        std::vector<Parameter> defaultParams = factory->m_parameters;

        auto ip = args.begin();
        ip++; // skip name
        for (; ip != args.end(); ++ip)
        {
            // split param from value : "param=value"
            auto paramValue = split(*ip, '=');

            if (paramValue.size() == 2)
            {
                // find param with name paramValue[0]
                auto paramName = paramValue[0];
                auto value = paramValue[1];
                // infer type of value
                auto paramType = ParamType::Integer;
                if (value == "true" || value == "false")
                    paramType = ParamType::Boolean;
                auto matchParam = [paramName](Parameter& p)->bool { return (p.m_name == paramName); };
                auto pv = std::find_if(defaultParams.begin(), defaultParams.end(), matchParam);
                if (pv != defaultParams.end())
                {
                    if (pv->m_paramType != paramType)
                    {
                        // invalid parameter
                        continue;
                    }
                    if (paramType == ParamType::Boolean)
                    {
                        if (value == "true")
                            pv->m_boolValue = true;
                        else
                            pv->m_boolValue = false;
                    }
                    else if (paramType == ParamType::Integer)
                    {
                        try
                        {
                            pv->m_intValue = std::stoi(value);
                        }
                        catch (std::invalid_argument)
                        {
                            pv->m_intValue = factory->m_enumStringToInt(value);
                        }
                    }
                }
            }
        }
        unit = UnitFactory::Create(unitName, defaultParams);
    }
    return unit;
}
