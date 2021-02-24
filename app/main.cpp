/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <Board.h>
#include <Dice.h>
#include <Battle.h>
#include <UnitFactory.h>
#include <AoSKeywords.h>
#include <codecvt>
#include "cxxopts.hpp"

void displayUnits(Verbosity verbose, const std::string& faction);
void displayWeapons(const std::string& unit);
void InitializeUnitMap();
Unit* GenerateRandomUnit(Keyword faction);
Keyword GenerateRandomFaction();

int main(int argc, char* argv[])
{
    int numRounds = 5;
    int verboseLevel = 0; // Verbosity::Silence == 0
    bool listUnits = false;
    std::string listFaction("all");
    std::string listWeapons("none");

    int numIterations = 1;
    bool saveMaps = true;
    std::string mapBaseName("battlemap");

    cxxopts::Options options(argv[0], "Age of Sigmar: Mano a Mano simulation.");
    options.add_options()
        ("h, help", "Print help")
        ("l, list", "List supported units")
        ("f, faction", "List units the given faction", cxxopts::value<std::string>(), "all")
        ("r, rounds", "Number of battle rounds", cxxopts::value<int>(numRounds))
        ("v, verbose", "Verbosity level", cxxopts::value<int>(verboseLevel))
        ("1, red", "Player 1 (Red) Unit", cxxopts::value<std::string>(), "")
        ("2, blue", "Player 2 (Blue) Unit", cxxopts::value<std::string>(), "")
        ("s, save", "Save battlemaps")
        ("mapname", "Battlemap basename", cxxopts::value<std::string>(mapBaseName))
        ("i, iterations", "Number of battle iterations", cxxopts::value<int>(numIterations))
        ("w, weapons", "List weapons for the given unit", cxxopts::value<std::string>(), "none")
        ;
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }
    if (result.count("list")) {
        listUnits = true;
    }
    if (result.count("faction")) {
        listFaction = result["faction"].as<std::string>();
    }
    if (result.count("save")) {
        saveMaps = true;
    }
    if (result.count("weapons")) {
        listWeapons = result["weapons"].as<std::string>();
    }

    Verbosity verbosity = Verbosity::Narrative;
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

    if (listUnits) {
        displayUnits(verbosity, listFaction);
        return EXIT_SUCCESS;
    }

    if (listWeapons != "none") {
        displayWeapons(listWeapons);
        return EXIT_SUCCESS;
    }

    Battle battle;
    battle.setNumRounds(numRounds);

    auto board = Board::Instance();

    auto pRedRoster = std::make_shared<Roster>(PlayerId::Red);
    auto pBlueRoster = std::make_shared<Roster>(PlayerId::Blue);

    auto pRedPlayer = std::make_shared<Player>(PlayerId::Red);
    pRedPlayer->setRoster(pRedRoster);

    auto pBluePlayer = std::make_shared<Player>(PlayerId::Blue);
    pBluePlayer->setRoster(pBlueRoster);

    auto redFaction = GenerateRandomFaction();
    auto blueFaction = GenerateRandomFaction();

    for (auto i = 0; i < 10; i++) {
        auto pRed = std::shared_ptr<Unit>(GenerateRandomUnit(redFaction));
        auto pBlue = std::shared_ptr<Unit>(GenerateRandomUnit(blueFaction));
        pRedRoster->addUnit(pRed);
        pBlueRoster->addUnit(pBlue);
    }

    Board::Instance()->setSize(72, 48);
    Board::Instance()->addRosters(pRedRoster, pBlueRoster);

    battle.addPlayers(pRedPlayer, pBluePlayer);

    std::cout << "Red Points: " << pRedRoster->getPoints() << "   Blue Points: " << pBlueRoster->getPoints() << std::endl;

    std::stringstream fn;

    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    SetVerbosity(verbosity);

    battle.deployment();

    if (saveMaps) {
        fn.str("");
        fn << mapBaseName << "_start.png";
        board->render(fn.str());
    }

    for (auto i = 0; i < numIterations; i++) {
        pRedRoster->restore();
        pBlueRoster->restore();

        battle.start(PlayerId::Red);

        while (!battle.done()) {
            battle.simulate();

            auto round = battle.currentRound();

            if (saveMaps && battle.currentPhase() == Phase::Battleshock) {
                fn.str("");
                fn << mapBaseName << "_round_" << round << ".png";
                board->render(fn.str());
            }

            battle.next();
        }

    }

    return EXIT_SUCCESS;
}

void displayUnits(Verbosity verbose, const std::string& faction) {
    bool listAll = (faction == "all");

    std::cout << "Supported Units in Faction(" << faction << "):" << std::endl;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip) {
        if (!listAll) {
            auto ki = FactionStringToKeyword(faction);
            if (ki != UNKNOWN) {
                // filter based on keyword
                for (auto fip : ruip->second.m_factions) {
                    if (ki != fip)
                        continue;
                }
            }
            else {
                // not found
                std::cout << "Faction " << faction << " not found." << std::endl;
                break;
            }
        }

        std::cout << "\t" << ruip->first << std::endl;

        if (verbose == Verbosity::Normal) {
            if (ruip->second.m_parameters.empty()) {
                std::cout << "\t  Parameters:  None" << std::endl;
                continue;
            }

            std::cout << "\t  Parameters:" << std::endl;
            for (auto pip : ruip->second.m_parameters) {
                if (pip.paramType == ParamType::Integer || pip.paramType == ParamType::Enum) {
                    if (ruip->second.m_paramToString == nullptr) {
                        if (pip.increment > 0) {
                            std::cout << "\t\tName: " << std::string(pip.name) << " Type: Integer  Value: "
                                      << pip.intValue << "  Allowed Values: ";
                            for (auto v = pip.minValue; v <= pip.maxValue; v += pip.increment) {
                                std::cout << v;
                                if (v < pip.maxValue) std::cout << ", ";
                            }
                            std::cout << std::endl;
                        }
                        else {
                            std::cout << "\t\tName: " << std::string(pip.name) << " Type: Integer  Value: "
                                      << pip.intValue
                                      << "  Min: " << pip.minValue << "  Max: " << pip.maxValue
                                      << std::endl;
                        }
                    }
                    else if (pip.increment > 0) {
                        std::cout << "\t\tName: " << std::string(pip.name) << " Type: Integer  Value: "
                                  << ruip->second.m_paramToString(pip) << "  Allowed Values: ";
                        Parameter parm(pip);
                        for (auto v = pip.minValue; v <= pip.maxValue; v += pip.increment) {
                            parm.intValue = v;
                            std::cout << ruip->second.m_paramToString(parm);
                            if (v < pip.maxValue) std::cout << ", ";
                        }
                        std::cout << std::endl;
                    }
                    else {
                        std::cout << "\t\tName: " << std::string(pip.name) << " Type: Integer  Value: "
                                  << ruip->second.m_paramToString(pip) << "  Min: " << pip.minValue
                                  << "  Max: " << pip.maxValue << std::endl;
                    }
                }
                else if (pip.paramType == ParamType::Boolean) {
                    std::cout << "\t\tName: " << std::string(pip.name) << " Type: Boolean  Value: "
                              << (pip.intValue ? "true" : "false") << std::endl;
                }
            }
        }
    }
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }
    return result;
}

std::string damageToString(int damage) {
    if (damage < 0)
    {
        switch (damage)
        {
            case RAND_D3:
                return "D3";
            case RAND_2D3:
                return "2D3";
            case RAND_D6:
                return "D6";
            case RAND_2D6:
                return "2D6";
            case RAND_3D6:
                return "3D6";
            case RAND_4D6:
                return "4D6";
            default:
                break;
        }
    }
    return std::string(std::to_string(damage));
}

void displayWeapons(const std::string& unitName)
{
    std::function<void(const Weapon&)> weaponVistor = [](const Weapon& weapon) {
        std::cout << "        " << weapon.name() << "  Type: " << (weapon.isMissile() ? "Missile" : "Melee")
                  << "  Range: " << weapon.range() << " Attacks: " << weapon.attacks()
                  << " To Hit: " << weapon.toHit() << "+  To Wound: " << weapon.toWound()
                  << "+  Rend: " << weapon.rend() << "  Damage: " << damageToString(weapon.damage())
                  << " Strength: " << weapon.strength() << std::endl;
    };

    if (unitName == "none") return;
    bool listAll = (unitName == "all");

    if (listAll)
    {
        std::cout << "Weapons: " << std::endl;
        for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
        {
            auto unit = UnitFactory::Create(ruip->first, ruip->second.m_parameters);
            if (unit)
            {
                std::cout << "    Unit: " << unit->name() << std::endl;
                unit->visitWeapons(weaponVistor);
                delete unit;
            }
        }
    }
    else
    {
        auto factory = UnitFactory::LookupUnit(unitName);
        if (factory)
        {
            auto unit = UnitFactory::Create(unitName, factory->m_parameters);
            if (unit)
            {
                std::cout << "Weapons: " << std::endl;
                std::cout << "    Unit: " << unit->name() << std::endl;
                unit->visitWeapons(weaponVistor);
                delete unit;
            }
        }
    }
}

std::map<int, std::string> g_unitMap;
std::vector<Keyword> g_factions;

void InitializeUnitMap()
{
    int unitId = 0;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        g_unitMap.insert(std::pair<int, std::string>(unitId, ruip->first));
        unitId++;
    }

    g_factions = {STORMCAST_ETERNAL,
            KHORNE,
            SYLVANETH,
            GLOOMSPITE_GITZ,
            MOONCLAN,
            NIGHTHAUNT,
            DAUGHTERS_OF_KHAINE,
            IDONETH_DEEPKIN,
            BEASTS_OF_CHAOS,
            SLAANESH,
            TZEENTCH,
            NURGLE,
            SLAVES_TO_DARKNESS,
            FLESH_EATER_COURTS,
            BONESPLITTERZ,
            GREENSKINZ,
            IRONJAWZ,
            FYRESLAYERS,
            KHARADRON_OVERLORDS,
            SERAPHON,
            SKAVEN,
            CITIES_OF_SIGMAR,
            OGOR_MAWTRIBES,
            OSSIARCH_BONEREAPERS,
            LUMINETH_REALM_LORDS,
            SONS_OF_BEHEMAT};
}

Unit* GenerateRandomUnit(Keyword faction) {
    const auto numUnits = (int)g_unitMap.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, numUnits-1);
    std::uniform_int_distribution<int> allInts(0, INT32_MAX);

    std::string name;
    const FactoryMethod* factory = nullptr;
    bool found = false;
    while (!found) {
        auto unitId = distribution(gen);
        name = g_unitMap.at(unitId);
        factory = UnitFactory::LookupUnit(name);
        if (std::find(factory->m_factions.begin(), factory->m_factions.end(), faction) != std::end(factory->m_factions)) {
            found = true;
        }
    }
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

Keyword GenerateRandomFaction() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, g_factions.size()-1);

    return KHORNE; // g_factions[distribution(gen)];
}
