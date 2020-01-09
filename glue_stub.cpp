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

const char* JSInterface::ParameterValueToString(const Parameter& param)
{
    // TODO: review this - who owns the returned char*?
    auto str = ::ParameterValueToString(param);
    return strdup(str.c_str());
}

#include "aossim_glue.cpp"