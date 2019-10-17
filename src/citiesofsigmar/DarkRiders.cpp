/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/DarkRiders.h"

namespace CitiesOfSigmar
{

Unit *DarkRiders::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string DarkRiders::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int DarkRiders::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void DarkRiders::Init()
{

}

DarkRiders::DarkRiders()
{

}

bool DarkRiders::configure()
{
    return false;
}

void DarkRiders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}