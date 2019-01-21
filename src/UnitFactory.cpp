/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "UnitFactory.h"


std::map<std::string, FactoryMethod> UnitFactory::s_registeredUnits = { };

bool UnitFactory::Register(const std::string& name, FactoryMethod factoryMethod)
{
    auto registeredPair = UnitFactory::s_registeredUnits.insert(std::make_pair(name.c_str(), factoryMethod));
    return registeredPair.second;
}

Unit* UnitFactory::Create(const std::string& name, const std::vector<Parameter>& parameters)
{
    auto registeredPair = UnitFactory::s_registeredUnits.find(name);

    if (registeredPair == UnitFactory::s_registeredUnits.end())
        return nullptr;

    return registeredPair->second.m_create(parameters);
}

