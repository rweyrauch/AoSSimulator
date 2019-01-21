/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <WarhammerSim.h>
#include <sstream>


std::vector<Parameter>::const_iterator FindParam(const std::string& name, const ParameterList &parameters)
{
    auto matchName = [name](const Parameter& param)->bool { return (param.m_name == name); };
    auto pip = std::find_if(parameters.begin(), parameters.end(), matchName);
    return pip;
}

int GetIntParam(const std::string& name, const ParameterList& parameters, int defaultValue)
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

bool GetBoolParam(const std::string& name, const ParameterList& parameters, bool defaultValue)
{
    bool value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end())
    {
        if (pip->m_paramType == ParamType::Boolean)
        {
            value = pip->m_boolValue;
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
    if (param.m_paramType == ParamType::Integer)
        ss << param.m_intValue;
    else if (param.m_paramType == ParamType::Boolean)
        ss << param.m_boolValue ? "true" : "false";
    return ss.str();
}
