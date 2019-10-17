/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildGeneral.h"

namespace CitiesOfSigmar
{

Unit *FreeguildGeneral::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildGeneral::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildGeneral::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildGeneral::Init()
{

}

FreeguildGeneral::FreeguildGeneral()
{

}

bool FreeguildGeneral::configure()
{
    return false;
}

void FreeguildGeneral::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}