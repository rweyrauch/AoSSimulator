/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildHandgunners.h"

namespace CitiesOfSigmar
{

Unit *FreeguildHandgunners::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildHandgunners::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildHandgunners::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildHandgunners::Init()
{

}

FreeguildHandgunners::FreeguildHandgunners()
{

}

bool FreeguildHandgunners::configure()
{
    return false;
}

void FreeguildHandgunners::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}