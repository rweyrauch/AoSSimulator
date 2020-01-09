/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sstream>
#include <UnitFactory.h>

static void LogUnitDescriptor(FactoryMethod* factory, const std::string &name, const std::vector<Parameter> &parameters)
{
    std::stringstream descriptor;
    descriptor << "\"" << name;
    for (auto ip : parameters)
    {
        descriptor << "," << ip.name << "=";
        if (ip.paramType == ParamType::Enum)
            descriptor << factory->m_paramToString(ip);
        else if (ip.paramType == ParamType::Integer)
            descriptor << ip.intValue;
        else if (ip.paramType == ParamType::Boolean)
        {
            if (ip.intValue != 0)
                descriptor << "true";
            else
                descriptor << "false";
        }
    }
    descriptor << "\"";
    SimLog(Verbosity::Debug, "%s\n", descriptor.str().c_str());
}

std::map<std::string, FactoryMethod> UnitFactory::s_registeredUnits = {};

bool UnitFactory::Register(const std::string &name, FactoryMethod factoryMethod)
{
    auto registeredPair = UnitFactory::s_registeredUnits.insert(std::make_pair(name.c_str(), factoryMethod));
    return registeredPair.second;
}

Unit *UnitFactory::Create(const std::string &name, const std::vector<Parameter> &parameters)
{
    auto registeredPair = UnitFactory::s_registeredUnits.find(name);

    if (registeredPair == UnitFactory::s_registeredUnits.end())
    {
        return nullptr;
    }

    LogUnitDescriptor(&registeredPair->second, name, parameters);

    return registeredPair->second.m_create(parameters);
}


const FactoryMethod *UnitFactory::LookupUnit(const std::string &name)
{
    auto ip = s_registeredUnits.find(name);
    if (ip == s_registeredUnits.end())
    {
        return nullptr;
    }
    return &(ip->second);
}

