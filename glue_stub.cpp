#include "include/AgeOfSigmarSim.h"
#include "include/AgeOfSigmarSimJS.h"
#include "include/ManoAMano.h"
#include "include/Unit.h"
#include "include/UnitFactory.h"

static std::vector<std::string> g_unitNames;

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

Unit* JSInterface::CreateUnit(const char* name, const Parameter* parameters, int numParams)
{
    const std::vector<Parameter> paramList(parameters, parameters+numParams);
    return UnitFactory::Create(std::string(name), paramList);
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

const char* JSInterface::UnitParameterValueToString(const char* name, const Parameter& parameter)
{
    auto factory = UnitFactory::LookupUnit(std::string(name));
    if (factory)
    {
        // TODO: review this - who owns the returned char*?
        auto value = factory->m_paramToString(parameter);
        return strdup(value.c_str());
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

#include "aossim_glue.cpp"