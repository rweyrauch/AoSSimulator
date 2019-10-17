/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildPistoliers.h"

namespace CitiesOfSigmar
{

Unit *FreeguildPistoliers::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildPistoliers::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildPistoliers::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildPistoliers::Init()
{

}

FreeguildPistoliers::FreeguildPistoliers()
{

}

bool FreeguildPistoliers::configure()
{
    return false;
}

void FreeguildPistoliers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}