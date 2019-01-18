/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNITFACTORY_H
#define UNITFACTORY_H

#include <map>
#include <vector>
#include "WarhammerSim.h"

class Unit;

typedef Unit* (*CreateMethod)(const std::vector<Parameter>& parameters);

struct FactoryMethod
{
public:
    CreateMethod m_create;
    std::vector<Parameter> m_parameters;
};

class UnitFactory
{
public:

    static bool Register(std::string name, FactoryMethod factoryMethod);

    static Unit* Create(std::string name, const std::vector<Parameter>& parameters);

    static std::map<std::string, FactoryMethod>::const_iterator RegisteredUnitsBegin() { return s_registeredUnits.begin(); }
    static std::map<std::string, FactoryMethod>::const_iterator RegisteredUnitsEnd() { return s_registeredUnits.end(); }

protected:

    static std::map<std::string, FactoryMethod> s_registeredUnits;
};

#endif // UNITFACTORY_H