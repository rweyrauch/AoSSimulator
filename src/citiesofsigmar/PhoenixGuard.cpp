/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/PhoenixGuard.h"

namespace CitiesOfSigmar
{

Unit *PhoenixGuard::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string PhoenixGuard::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int PhoenixGuard::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void PhoenixGuard::Init()
{

}

PhoenixGuard::PhoenixGuard()
{

}

bool PhoenixGuard::configure()
{
    return false;
}

void PhoenixGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}