/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <map>
#include <algorithm>
#include <AgeOfSigmarSim.h>
#include <sstream>
#include <cstdarg>


std::vector<Parameter>::const_iterator FindParam(const std::string &name, const ParameterList &parameters)
{
    auto matchName = [name](const Parameter &param) -> bool
    { return (param.m_name == name); };
    auto pip = std::find_if(parameters.begin(), parameters.end(), matchName);
    return pip;
}

std::vector<Parameter>::iterator FindParam(const std::string &name, ParameterList &parameters)
{
    auto matchName = [name](const Parameter &param) -> bool
    { return (param.m_name == name); };
    auto pip = std::find_if(parameters.begin(), parameters.end(), matchName);
    return pip;
}

int GetIntParam(const std::string &name, const ParameterList &parameters, int defaultValue)
{
    int value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end())
    {
        if (pip->m_paramType == ParamType::Integer)
        {
            value = pip->m_intValue;
        }
    }
    return value;
}

int GetEnumParam(const std::string &name, const ParameterList &parameters, int defaultValue)
{
    int value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end())
    {
        if (pip->m_paramType == ParamType::Enum)
        {
            value = pip->m_intValue;
        }
    }
    return value;
}

bool GetBoolParam(const std::string &name, const ParameterList &parameters, bool defaultValue)
{
    bool value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end())
    {
        if (pip->m_paramType == ParamType::Boolean)
        {
            value = (pip->m_intValue == 0) ? false : true;
        }
    }
    return value;
}

std::string PhaseToString(Phase phase)
{
    switch (phase)
    {
        case Phase::Initiative:
            return "Initiative";
        case Phase::Hero:
            return "Hero";
        case Phase::Movement:
            return "Movement";
        case Phase::Shooting:
            return "Shooting";
        case Phase::Charge:
            return "Charge";
        case Phase::Combat:
            return "Combat";
        case Phase::Battleshock:
            return "Battleshock";
    }
    return "";
}

std::string PlayerIdToString(PlayerId id)
{
    switch (id)
    {
        case PlayerId::None:
            return "None";
        case PlayerId::Red:
            return "Red";
        case PlayerId::Blue:
            return "Blue";
    }
    return "";
}

float AverageRandomValue(int value)
{
    float avgValue = value;
    switch (value)
    {
        case RAND_D3:
            avgValue = 2.0f;
            break;
        case RAND_D6:
            avgValue = 3.5;
            break;
        case RAND_2D6:
            avgValue = 3.5f * 2;
            break;
        case RAND_3D6:
            avgValue = 3.5f * 3;
            break;
        case RAND_4D6:
            avgValue = 3.5f * 4;
            break;
        default:
            avgValue = value;
            break;
    }
    return avgValue;
}

std::string ParameterValueToString(const Parameter &param)
{
    std::stringstream ss;
    if (param.m_paramType == ParamType::Integer || param.m_paramType == ParamType::Enum)
    {
        ss << param.m_intValue;
    }
    else if (param.m_paramType == ParamType::Boolean)
    {
        if (param.m_intValue != 0)
            ss << "true";
        else
            ss << "false";
    }
    return ss.str();
}

PlayerId GetEnemyId(PlayerId friendlyId)
{
    if (friendlyId == PlayerId::Red)
    {
        return PlayerId::Blue;
    }
    else if (friendlyId == PlayerId::Blue)
    {
        return PlayerId::Red;
    }
    return PlayerId::None;
}


static std::map<std::string, Keyword> g_allianceNameLookup = {
    { "Order", ORDER },
    { "Chaos", CHAOS },
    { "Death", DEATH },
    { "Destruction", DESTRUCTION }
};

static std::map<std::string, Keyword> g_factionNameLookup = {
    { "Stormcast Eternal", STORMCAST_ETERNAL, },
    { "Khorne", KHORNE },
    { "Sylvaneth", SYLVANETH },
    { "Gloomspite Gitz", GLOOMSPITE_GITZ },
    { "Moonclan", MOONCLAN },
    { "Nighthaunt", NIGHTHAUNT },
    { "Daughters of Khaine", DAUGHTERS_OF_KHAINE },
    { "Idoneth Deepkin", IDONETH_DEEPKIN },
    { "Beasts of Chaos", BEASTS_OF_CHAOS },
    { "Slaanesh", SLAANESH },
    { "Tzeentch", TZEENTCH },
    { "Nurgle", NURGLE },
    { "Slaves to Darkness", SLAVES_TO_DARKNESS },
    { "Flesh Eater Courts", FLESH_EATER_COURTS },
    { "Grand Host of Nagash", GRAND_HOST_OF_NAGASH },
    { "Legion of Blood", LEGION_OF_BLOOD },
    { "Legion of Night", LEGION_OF_NIGHT },
    { "Legion of Sacrament", LEGION_OF_SACRAMENT },
    { "Soulblight", SOULBLIGHT },
    { "Beastclaw Raiders", BEASTCLAW_RAIDERS },
    { "Bonesplitterz", BONESPLITTERZ },
    { "Greenskinz", GREENSKINZ },
    { "Ironjawz", IRONJAWZ },
    { "Darkling Covens", DARKLING_COVENS },
    { "Devoted of Sigmar", DEVOTED_OF_SIGMAR },
    { "Dispossessed", DISPOSSESSED },
    { "Eldritch Council", ELDRITCH_COUNCIL },
    { "Free Peoples", FREE_PEOPLES },
    { "Fyreslayers", FYRESLAYERS },
    { "Kharadron Overlords", KHARADRON_OVERLORDS },
    { "Order Draconis", ORDER_DRACONIS },
    { "Order Serpentis", ORDER_SERPENTIS },
    { "Phoenix Temple", PHOENIX_TEMPLE },
    { "Scourge Privateers", SCOURGE_PRIVATEERS },
    { "Seraphon", SERAPHON },
    { "Shadowblades", SHADOWBLADES },
    { "Swifthawk Agents", SWIFTHAWK_AGENTS },
    { "Wanderers", WANDERER },
    { "Skaven", SKAVEN },
    { "Deathrattle", DEATHRATTLE },
    { "Deadwalkers", DEADWALKERS},
    { "Deathmages", DEATHMAGES },
    { "Deathlords", DEATHLORDS },
    { "Everchosen", EVERCHOSEN },
    { "Free Agent", FREE_AGENT },
    { "Bonesplitterz", BONESPLITTERZ }
};

Keyword grandAllianceStringToKeyword(const std::string &allianceName)
{
    auto ki = g_allianceNameLookup.find(allianceName);
    if (ki != g_factionNameLookup.end())
        return ki->second;
    return UNKNOWN;
}

Keyword factionStringToKeyword(const std::string &factionName)
{
    auto ki = g_factionNameLookup.find(factionName);
    if (ki != g_factionNameLookup.end())
        return ki->second;
    return UNKNOWN;
}

std::string factionKeywordToString(Keyword faction)
{
    for (const auto& ip : g_factionNameLookup)
    {
        if (ip.second == faction)
            return ip.first;
    }
    return "Not Found";
}

void SimLog(Verbosity verbosity, const char* format, ...)
{
    if ((int)verbosity >= (int)GetVerbosity())
        return;

    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

bool expired(const Duration& duration, const Duration& current)
{
    return ((duration.player == current.player) && (duration.round < current.round) && (duration.phase == current.phase));
}
