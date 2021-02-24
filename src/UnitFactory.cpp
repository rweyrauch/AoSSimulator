/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sstream>
#include <algorithm>
#include <UnitFactory.h>
#include <Unit.h>

Parameter BoolParameter(const char* name) {
    return {ParamType::Boolean, name, 0, 0, 1, 1, {}};
}

Parameter IntegerParameter(const char* name, int value, int min, int max, int incr) {
    return { ParamType::Integer, name, value, min, max, incr, {}};
}

std::vector<Parameter>::const_iterator FindParam(const std::string &name, const ParameterList &parameters) {
    auto matchName = [name](const Parameter &param) -> bool { return (param.name == name); };
    auto pip = std::find_if(parameters.begin(), parameters.end(), matchName);
    return pip;
}

std::vector<Parameter>::iterator FindParam(const std::string &name, ParameterList &parameters) {
    auto matchName = [name](const Parameter &param) -> bool { return (param.name == name); };
    auto pip = std::find_if(parameters.begin(), parameters.end(), matchName);
    return pip;
}

int GetIntParam(const std::string &name, const ParameterList &parameters, int defaultValue) {
    int value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end()) {
        if (pip->paramType == ParamType::Integer) {
            value = pip->intValue;
        }
    }
    return value;
}

int GetEnumParam(const std::string &name, const ParameterList &parameters, int defaultValue) {
    int value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end()) {
        if (pip->paramType == ParamType::Enum) {
            value = pip->values[pip->intValue];
        }
    }
    return value;
}

bool GetBoolParam(const std::string &name, const ParameterList &parameters, bool defaultValue) {
    bool value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end()) {
        if (pip->paramType == ParamType::Boolean) {
            value = (pip->intValue != 0);
        }
    }
    return value;
}

std::string ParameterValueToString(const Parameter &param) {
    std::stringstream ss;
    if (param.paramType == ParamType::Integer) {
        ss << param.intValue;
    } else if (param.paramType == ParamType::Enum) {
        ss << param.values[param.intValue];
    } else if (param.paramType == ParamType::Boolean) {
        if (param.intValue != 0)
            ss << "true";
        else
            ss << "false";
    }
    return ss.str();
}

static void
LogUnitDescriptor(FactoryMethod *factory, const std::string &name, const std::vector<Parameter> &parameters) {
    std::stringstream descriptor;
    descriptor << "\"" << name;
    for (auto ip : parameters) {
        descriptor << "," << ip.name << "=";
        if (ip.paramType == ParamType::Enum)
            descriptor << factory->m_paramToString(ip);
        else if (ip.paramType == ParamType::Integer)
            descriptor << ip.intValue;
        else if (ip.paramType == ParamType::Boolean) {
            if (ip.intValue != 0)
                descriptor << "true";
            else
                descriptor << "false";
        }
    }
    descriptor << "\"";
    PLOG_INFO.printf("%s\n", descriptor.str().c_str());
}

std::map<std::string, FactoryMethod> UnitFactory::s_registeredUnits = {};

bool UnitFactory::Register(const std::string &name, const FactoryMethod &factoryMethod) {
    auto registeredPair = UnitFactory::s_registeredUnits.insert(std::make_pair(name.c_str(), factoryMethod));
    return registeredPair.second;
}

Unit *UnitFactory::Create(const std::string &name, const std::vector<Parameter> &parameters) {
    auto registeredPair = UnitFactory::s_registeredUnits.find(name);

    if (registeredPair == UnitFactory::s_registeredUnits.end()) {
        return nullptr;
    }

    //fprintf(stderr, "Unit: %s  Num Params: %d\n", name.c_str(), (int)parameters.size());
    //for (auto ip : parameters)
    //{
    //    fprintf(stderr, "\tType: %d  Name: %s  Value: %d\n", ip.paramType, ip.name, ip.intValue);
    //}

    LogUnitDescriptor(&registeredPair->second, name, parameters);

    auto unit = registeredPair->second.m_create(parameters);
    return unit;
}


const FactoryMethod *UnitFactory::LookupUnit(const std::string &name) {
    auto ip = s_registeredUnits.find(name);
    if (ip == s_registeredUnits.end()) {
        return nullptr;
    }
    return &(ip->second);
}

