/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildOutriders.h"

namespace CitiesOfSigmar
{

Unit *FreeguildOutriders::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildOutriders::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildOutriders::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildOutriders::Init()
{

}

FreeguildOutriders::FreeguildOutriders()
{

}

bool FreeguildOutriders::configure()
{
    return false;
}

void FreeguildOutriders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}