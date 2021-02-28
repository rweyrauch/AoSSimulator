/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include "AgeOfSigmarSim.h"
#include "ManoAMano.h"
#include "Unit.h"
#include "UnitFactory.h"

static std::vector<std::string> g_unitNames;

struct JSUnitInfo {
public:
    const char* name;
    int numberOfParameters;
    void getParameter(int which, Parameter& param) { param = this->parameters[which]; }
    int grandAlliance;
    int numberOfFactions;
    int getFaction(int which) { return this->factions[which]; }
    const Parameter* parameters;
    const int* factions;
};

class JSInputParameter {
public:
    JSInputParameter(ParamType type, const char* n) : paramType(type), name(nullptr) {
        name = strdup(n);
    }

    ParamType paramType = Integer;
    char* name = nullptr;
    int intValue = 0;
    int minValue = 0;
    int maxValue = 0;
    int increment = 1;
};

class JSInterface {
public:
    static void Initialize();
    static int GrandAllianceStringToKeyword(const char* allianceName);
    static int FactionStringToKeyword(const char* factionName);
    static const char* FactionKeywordToString(int faction);
    static const char* GrandAllianceKeywordToString(int ga);

    static void CreateUnit(PlayerId which, const char* name);
    static int AddUnitParameter(PlayerId which, Parameter& parameter);
    static void ClearUnitParameters(PlayerId which);

    static Unit* GetUnit(PlayerId which);

    static void BattleStart(int numRounds);
    static void BattleSimulate();
    static void BattleNext();
    static bool BattleDone();
    static long BattleCurrentRound();
    static void GetStatistics(PlayerId which, UnitStatistics& stats);
    static void ClearStatistics();
    static PlayerId GetVictor();
    static float GetInitialRedX();
    static float GetInitialRedY();
    static float GetInitialBlueX();
    static float GetInitialBlueY();
    static float GetBoardWidth();
    static float GetBoardDepth();

    static int GetNumberOfAvailableUnits();
    static void GetUnitInfo(int which, JSUnitInfo& info);
    static void GetUnitInfoByName(const char* name, JSUnitInfo& info);
    static const char* UnitParameterValueToString(const char* unitName, const char* paramName, int value);
    static int UnitEnumStringToInt(const char* name, const char* enumString);
    static int GetUnitPoints(const char* name, int numModels);
};


void JSInterface::Initialize() {
    ::Initialize(plog::error);

    for (auto ip = UnitFactory::RegisteredUnitsBegin(); ip != UnitFactory::RegisteredUnitsEnd(); ++ip) {
        auto name = ip->first;
        g_unitNames.push_back(name);
    }
}

int JSInterface::GrandAllianceStringToKeyword(const char* allianceName) {
    return ::GrandAllianceStringToKeyword(std::string(allianceName));
}

int JSInterface::FactionStringToKeyword(const char* factionName) {
    return ::FactionStringToKeyword(std::string(factionName));
}

const char* JSInterface::GrandAllianceKeywordToString(int ga) {
    // Returned string is a const.
    const auto& str = ::GrandAllianceKeywordToString((Keyword)ga);
    return str.c_str();
}

const char* JSInterface::FactionKeywordToString(int faction) {
    // Returned string is a const.
    const auto& str = ::FactionKeywordToString((Keyword)faction);
    return str.c_str();
}

static std::vector<Parameter> g_paramList[2];
static std::shared_ptr<Unit> g_redUnit = nullptr;
static std::shared_ptr<Unit> g_blueUnit = nullptr;
static std::shared_ptr<ManoAMano> g_battle = nullptr;

void JSInterface::CreateUnit(PlayerId which, const char* name) {
    if (which == PlayerId::Red)
        g_redUnit = std::shared_ptr<Unit>(UnitFactory::Create(std::string(name), g_paramList[0]));
    else if (which == PlayerId::Blue)
        g_blueUnit = std::shared_ptr<Unit>(UnitFactory::Create(std::string(name), g_paramList[1]));
}

int JSInterface::AddUnitParameter(PlayerId which, Parameter& parameter) {
    const int index = (which == PlayerId::Red) ? 0 : 1;
    fprintf(stderr, "Param:  Type: %d  Name: %s  Value: %d\n", parameter.paramType, parameter.name, parameter.intValue);
    Parameter param = parameter;
    param.name = strdup(parameter.name);
    g_paramList[index].push_back(param);
    return (int)g_paramList[index].size();
}

void JSInterface::ClearUnitParameters(PlayerId which) {
    const int index = (which == PlayerId::Red) ? 0 : 1;
    g_paramList[index].clear();
}

Unit* JSInterface::GetUnit(PlayerId which) {
    if (which == PlayerId::Red) return g_redUnit.get();
    else if (which == PlayerId::Blue) return g_blueUnit.get();
    return nullptr;
}

void JSInterface::BattleStart(int numRounds) {
    if ((g_redUnit == nullptr) || (g_blueUnit == nullptr)) {
        return;
    }
    g_battle = std::make_shared<ManoAMano>(numRounds, Realm::Azyr);
    g_battle->combatants(g_redUnit, g_blueUnit);
    g_battle->start();
}

void JSInterface::BattleSimulate() {
    if (g_battle) g_battle->simulate();
}

void JSInterface::BattleNext() {
    if (g_battle) g_battle->next();
}

bool JSInterface::BattleDone() {
    if (g_battle)
        return g_battle->done();
    return true;
}

long JSInterface::BattleCurrentRound() {
    if (g_battle)
        return g_battle->currentRound();
    return 0;
}

void JSInterface::GetStatistics(PlayerId which, UnitStatistics& stats) {
    if (g_battle)
        g_battle->getStatistics(which, stats);
}

void JSInterface::ClearStatistics() {
    if (g_battle)
        g_battle->clearStatistics();
}

PlayerId JSInterface::GetVictor() {
    if (g_battle)
        return g_battle->getVictor();
    return PlayerId::None;
}

float JSInterface::GetInitialRedX() {
    if (g_battle)
        return g_battle->getInitialRedX();
    return 0.0f;
}

float JSInterface::GetInitialRedY() {
    if (g_battle)
        return g_battle->getInitialRedY();
    return 0.0f;
}
float JSInterface::GetInitialBlueX() {
    if (g_battle)
        return g_battle->getInitialBlueX();
    return 0.0f;
}
float JSInterface::GetInitialBlueY() {
    if (g_battle)
        return g_battle->getInitialBlueY();
    return 0.0f;
}
float JSInterface::GetBoardWidth() {
    if (g_battle)
        return g_battle->getBoardWidth();
    return 1.0f;
}
float JSInterface::GetBoardDepth() {
    if (g_battle)
        return g_battle->getBoardDepth();
    return 1.0f;
}

int JSInterface::GetNumberOfAvailableUnits() {
    return (int)g_unitNames.size();
}

void JSInterface::GetUnitInfo(int which, JSUnitInfo& info) {
    const auto& name = g_unitNames[which];
    GetUnitInfoByName(name.c_str(), info);
}

void JSInterface::GetUnitInfoByName(const char* name, JSUnitInfo& info) {
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory) {
        info.name = name;
        info.grandAlliance = factory->m_grandAlliance;
        info.numberOfParameters = (int)factory->m_parameters.size();
        info.parameters = factory->m_parameters.data();
        info.numberOfFactions = (int)factory->m_factions.size();
        info.factions = (const int*)factory->m_factions.data();
    }
}

const char* JSInterface::UnitParameterValueToString(const char* unitName, const char* paramName, int value) {
    auto factory = UnitFactory::LookupUnit(std::string(unitName));
    if (factory) {
        // TODO: review this - who owns the returned char*?
        Parameter parameter;
        parameter.name = paramName;
        parameter.intValue = value;
        auto strValue = factory->m_paramToString(parameter);
        //std::cout << "Unit: " << unitName << "  Param Name: " << paramName << "  Value: " << value << "  AsString: " << strValue << std::endl;
        return strdup(strValue.c_str());
    }
    return nullptr;
}

int JSInterface::UnitEnumStringToInt(const char* name, const char* enumString) {
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory) {
        auto value = factory->m_enumStringToInt(std::string(enumString));
        return value;
    }
    return 0;
}

int JSInterface::GetUnitPoints(const char* name, int numModels) {
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory) {
        auto points = factory->m_computePoints(numModels);
        return points;
    }
    return 0;
}

#include "aossim_glue.cpp"