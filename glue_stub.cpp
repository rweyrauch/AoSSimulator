/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include "include/AgeOfSigmarSim.h"
#include "include/ManoAMano.h"
#include "include/Unit.h"
#include "include/UnitFactory.h"

static std::vector<std::string> g_unitNames;

struct JSUnitInfo
{
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

class JSInputParameter
{
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

class JSInterface
{
public:
    static void Initialize();
    static void SetVerbosity(Verbosity verbosity);
    static int GrandAllianceStringToKeyword(const char* allianceName);
    static int FactionStringToKeyword(const char* factionName);
    static const char* FactionKeywordToString(int faction);
    static const char* GrandAllianceKeywordToString(int ga);

    static Unit* CreateUnit(const char* name);
    static int AddUnitParameter(Parameter& parameter);
    static void ClearUnitParameters();
    static int GetNumberOfAvailableUnits();
    static void GetUnitInfo(int which, JSUnitInfo& info);
    static void GetUnitInfoByName(const char* name, JSUnitInfo& info);
    static const char* UnitParameterValueToString(const char* unitName, const char* paramName, int value);
    static int UnitEnumStringToInt(const char* name, const char* enumString);
    static int GetUnitPoints(const char* name, int numModels);
};


void JSInterface::Initialize()
{
    ::Initialize(Verbosity::Normal);

    for (auto ip = UnitFactory::RegisteredUnitsBegin(); ip != UnitFactory::RegisteredUnitsEnd(); ++ip)
    {
        auto name = ip->first;
        g_unitNames.push_back(name);
    }
}

void JSInterface::SetVerbosity(Verbosity verbosity)
{
    ::SetVerbosity(verbosity);
}

int JSInterface::GrandAllianceStringToKeyword(const char* allianceName)
{
    return ::GrandAllianceStringToKeyword(std::string(allianceName));
}

int JSInterface::FactionStringToKeyword(const char* factionName)
{
    return ::FactionStringToKeyword(std::string(factionName));
}

const char* JSInterface::GrandAllianceKeywordToString(int ga)
{
    // Returned string is a const.
    const auto& str = ::GrandAllianceKeywordToString((Keyword)ga);
    return str.c_str();
}

const char* JSInterface::FactionKeywordToString(int faction)
{
    // Returned string is a const.
    const auto& str = ::FactionKeywordToString((Keyword)faction);
    return str.c_str();
}

static  std::vector<Parameter> g_paramList;

Unit* JSInterface::CreateUnit(const char* name)
{
    return UnitFactory::Create(std::string(name), g_paramList);
}

int JSInterface::AddUnitParameter(Parameter& parameter)
{
    fprintf(stderr, "Param:  Type: %d  Name: %s  Value: %d\n", parameter.paramType, parameter.name, parameter.intValue);
    Parameter param = parameter;
    param.name = strdup(parameter.name);
    g_paramList.push_back(param);
    return (int)g_paramList.size();
}

void JSInterface::ClearUnitParameters()
{
    g_paramList.clear();
}

int JSInterface::GetNumberOfAvailableUnits()
{
    return (int)g_unitNames.size();
}

void JSInterface::GetUnitInfo(int which, JSUnitInfo& info)
{
    const auto& name = g_unitNames[which];
    GetUnitInfoByName(name.c_str(), info);
}

void JSInterface::GetUnitInfoByName(const char* name, JSUnitInfo& info)
{
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory)
    {
        info.name = name;
        info.grandAlliance = factory->m_grandAlliance;
        info.numberOfParameters = (int)factory->m_parameters.size();
        info.parameters = factory->m_parameters.data();
        info.numberOfFactions = (int)factory->m_factions.size();
        info.factions = (const int*)factory->m_factions.data();
    }
}

const char* JSInterface::UnitParameterValueToString(const char* unitName, const char* paramName, int value)
{
    auto factory = UnitFactory::LookupUnit(std::string(unitName));
    if (factory)
    {
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

int JSInterface::UnitEnumStringToInt(const char* name, const char* enumString)
{
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory)
    {
        auto value = factory->m_enumStringToInt(std::string(enumString));
        return value;
    }
    return 0;
}

int JSInterface::GetUnitPoints(const char* name, int numModels)
{
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory)
    {
        auto points = factory->m_computePoints(numModels);
        return points;
    }
    return 0;
}

#include "aossim_glue.cpp"
