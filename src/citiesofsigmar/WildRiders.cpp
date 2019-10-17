/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/WildRiders.h"

namespace CitiesOfSigmar
{

Unit *WildRiders::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string WildRiders::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int WildRiders::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void WildRiders::Init()
{

}

WildRiders::WildRiders()
{

}

bool WildRiders::configure()
{
    return false;
}

void WildRiders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}