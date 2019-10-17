/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/DrakespawnKnights.h"

namespace CitiesOfSigmar
{

Unit *DrakespawnKnights::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string DrakespawnKnights::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int DrakespawnKnights::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void DrakespawnKnights::Init()
{

}

DrakespawnKnights::DrakespawnKnights()
{

}

bool DrakespawnKnights::configure()
{
    return false;
}

void DrakespawnKnights::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}