/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildCrossbowmen.h"

namespace CitiesOfSigmar
{

Unit *FreeguildCrossbowmen::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildCrossbowmen::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildCrossbowmen::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildCrossbowmen::Init()
{

}

FreeguildCrossbowmen::FreeguildCrossbowmen()
{

}

bool FreeguildCrossbowmen::configure()
{
    return false;
}

void FreeguildCrossbowmen::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}