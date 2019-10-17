/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildGuard.h"

namespace CitiesOfSigmar
{

Unit *FreeguildGuard::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildGuard::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildGuard::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildGuard::Init()
{

}

FreeguildGuard::FreeguildGuard()
{

}

bool FreeguildGuard::configure()
{
    return false;
}

void FreeguildGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}