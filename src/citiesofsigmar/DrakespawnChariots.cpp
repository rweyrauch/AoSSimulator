/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/DrakespawnChariots.h"

namespace CitiesOfSigmar
{

Unit *DrakespawnChariots::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string DrakespawnChariots::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int DrakespawnChariots::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void DrakespawnChariots::Init()
{

}

DrakespawnChariots::DrakespawnChariots()
{

}

bool DrakespawnChariots::configure()
{
    return false;
}

void DrakespawnChariots::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}