#include "include/AgeOfSigmarSim.h"
#include "include/AgeOfSigmarSimJS.h"
#include "include/ManoAMano.h"
#include "include/Unit.h"

void JSInterface::Initialize()
{
    ::Initialize(Verbosity::Normal);
}

int JSInterface::GrandAllianceStringToKeyword(const char* allianceName)
{
    return ::GrandAllianceStringToKeyword(std::string(allianceName));
}

int JSInterface::FactionStringToKeyword(const char* factionName)
{
    return ::FactionStringToKeyword(std::string(factionName));
}

const char* JSInterface::FactionKeywordToString(int faction)
{
    // Returned string is a const.
    const auto& str = ::FactionKeywordToString((Keyword)faction);
    return str.c_str();
}

const char* JSInterface::ParameterValueToString(const ParameterJS& param)
{
    Parameter localParam;
    localParam.m_name = std::string(param.name);
    localParam.m_paramType = param.paramType;
    localParam.m_intValue = param.intValue;
    localParam.m_minValue = param.minValue;
    localParam.m_maxValue = param.maxValue;
    localParam.m_increment = param.increment;

    // TODO: review this - who owns the returned char*?
    auto str = ::ParameterValueToString(localParam);
    return strdup(str.c_str());
}

#include "aossim_glue.cpp"