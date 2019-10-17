/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/WarHydra.h"

namespace CitiesOfSigmar
{

Unit *WarHydra::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string WarHydra::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int WarHydra::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void WarHydra::Init()
{

}

WarHydra::WarHydra()
{

}

bool WarHydra::configure()
{
    return false;
}

void WarHydra::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}