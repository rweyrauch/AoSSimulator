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

void InitializeUnitMap();
Unit* GenerateRandomUnit();

int main(int argc, char* argv[])
{
    int numRounds = 5;
    int verboseLevel = 0; // Verbosity::Silence == 0
    int numIterations = 10;

    cxxopts::Options options(argv[0], "Age of Sigmar: Mano a Mano simulation.");
    options.add_options()
        ("h, help", "Print help")
        ("r, rounds", "Number of battle rounds", cxxopts::value<int>(numRounds))
        ("v, verbose", "Enable verbose logging", cxxopts::value<int>(verboseLevel))
        ("i, iterations", "Number of battle iterations", cxxopts::value<int>(numIterations))
        ;
    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    Verbosity verbosity = Verbosity::Normal;
    if (verboseLevel == 0)
        verbosity = Verbosity::Silence;
    else if (verboseLevel == 1)
        verbosity = Verbosity::Normal;
    else if (verboseLevel == 2)
        verbosity = Verbosity::Debug;
    else if (verboseLevel == 3)
        verbosity = Verbosity::Narrative;

    Initialize(verbosity);

    InitializeUnitMap();

    for (int i = 0; i < numIterations; i++)
    {
        auto battle = std::make_unique<ManoAMano>(numRounds);

        auto pRed = std::shared_ptr<Unit>(GenerateRandomUnit());
        auto pBlue = std::shared_ptr<Unit>(GenerateRandomUnit());
        if (pRed == nullptr || pBlue == nullptr)
        {
            continue;
        }

        std::cout << "Battle " << i << std::endl;
        std::cout << "  Red: " << pRed->name() << "  NumModels: " << pRed->remainingModels() << " vs "
            << "  Blue: " << pBlue->name() << "  NumModels: " << pBlue->remainingModels() << std::endl;

        battle->combatants(pRed, pBlue);

        battle->start();

        while (!battle->done())
        {
            battle->simulate();
            battle->next();
        }

        auto victor = battle->getVictor();
        if (victor == PlayerId::Red)
            std::cout << "Team Red, " << pRed->name() << ", was victorious." << std::endl;
        else if (victor == PlayerId::Blue)
            std::cout << "Team Blue, " << pBlue->name() << ", was victorious." << std::endl;
        else
            std::cout << "Tie! " << std::endl;

    }

    return EXIT_SUCCESS;
}

std::map<int, std::string> g_unitMap;

void InitializeUnitMap()
{
    int unitId = 0;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        g_unitMap.insert(std::pair<int, std::string>(unitId, ruip->first));
        unitId++;
    }
}

Unit* GenerateRandomUnit()
{
    const auto numUnits = (int)g_unitMap.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, numUnits-1);
    std::uniform_int_distribution<int> allInts(0, INT32_MAX);

    const auto unitId = distribution(gen);

    auto name = g_unitMap.at(unitId);
    auto factory = UnitFactory::LookupUnit(name);
    auto parameters = factory->m_parameters;
    for (auto& ip : parameters)
    {
        if (ip.paramType == ParamType::Boolean)
        {
            int value = allInts(gen) % 2;
            ip.intValue = value;
        }
        else if (ip.paramType == ParamType::Integer)
        {
            int minValue = ip.minValue;
            if (ip.increment != 0) minValue /= ip.increment;
            int maxValue = ip.maxValue;
            if (ip.increment != 0) maxValue /= ip.increment;

            int valueRange = (maxValue - minValue);

            int value = allInts(gen) % valueRange + minValue;
            if (ip.increment != 0) value *= ip.increment;

            ip.intValue = value;
        }
        else if (ip.paramType == ParamType::Enum)
        {
            int value = allInts(gen) % ip.numValues;
            ip.intValue = value;
        }
    }

    auto unit = UnitFactory::Create(name, parameters);
    if (unit == nullptr)
    {
        for (auto& pp : parameters)
        {
            if (pp.paramType == ParamType::Integer && std::string(pp.name) != "Models")
                pp.intValue = 0;
        }

        unit = UnitFactory::Create(name, parameters);
        if (unit == nullptr)
        {
            std::cerr << "Failed to create unit " << name << "." << std::endl;
            std::cerr << "\tParameters: " << std::endl;
            for (auto &pp : parameters)
            {
                std::cerr << "\t\t" << std::string(pp.name) << ": " << pp.intValue << std::endl;
            }
        }
    }
    return unit;
}
