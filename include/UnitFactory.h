/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <map>
#include <vector>
#include "AgeOfSigmarSim.h"
#include "AoSKeywords.h"

enum ParamType {
    Boolean,
    Integer,
    Enum,
};

enum {
    Sim_False = 0,
    Sim_True = 1,
};

struct Parameter {
    ParamType paramType = Integer;
    const char *name = nullptr;
    int intValue = 0;
    int minValue = 0;
    int maxValue = 0;
    int increment = 1;
    int numValues = 0;
    const int *values = nullptr;
};

Parameter BoolParameter(const char* name);

Parameter IntegerParameter(const char* name, int value, int min, int max, int incr);

template<std::size_t N>
Parameter EnumParameter(const char* name, int value, const std::array<int, N>& values) {
    return { ParamType::Enum, name, value, 0, N-1, 1, (int)N, values.data()};
}

typedef std::vector<Parameter> ParameterList;

std::string ParameterValueToString(const Parameter &param);

ParameterList::const_iterator FindParam(const std::string &name, const ParameterList &parameters);

ParameterList::iterator FindParam(const std::string &name, ParameterList &parameters);

int GetIntParam(const std::string &name, const ParameterList &parameters, int defaultValue);

int GetEnumParam(const std::string &name, const ParameterList &parameters, int defaultValue);

bool GetBoolParam(const std::string &name, const ParameterList &parameters, bool defaultValue);


class Unit;

typedef Unit *(*CreateMethod)(const std::vector<Parameter> &parameters);

typedef std::string (*ParamValueToString)(const Parameter &parameter);

typedef int (*EnumStringToInt)(const std::string &enumString);

typedef int (*ComputePoints)(const std::vector<Parameter> &parameters);

struct FactoryMethod {
public:
    CreateMethod m_create;
    ParamValueToString m_paramToString;
    EnumStringToInt m_enumStringToInt;
    ComputePoints m_computePoints;
    std::vector<Parameter> m_parameters;
    Keyword m_grandAlliance;
    std::vector<Keyword> m_factions;
};

class UnitFactory {
public:

    static bool Register(const std::string &name, const FactoryMethod &factoryMethod);

    static Unit *Create(const std::string &name, const std::vector<Parameter> &parameters);

    static std::map<std::string, FactoryMethod>::const_iterator RegisteredUnitsBegin() { return s_registeredUnits.begin(); }

    static std::map<std::string, FactoryMethod>::const_iterator RegisteredUnitsEnd() { return s_registeredUnits.end(); }

    static const FactoryMethod *LookupUnit(const std::string &name);

protected:

    static std::map<std::string, FactoryMethod> s_registeredUnits;
};

