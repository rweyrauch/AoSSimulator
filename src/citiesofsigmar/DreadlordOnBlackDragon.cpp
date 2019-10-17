/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/DreadlordOnBlackDragon.h"

namespace CitiesOfSigmar
{

Unit *DreadlordOnBlackDragon::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string DreadlordOnBlackDragon::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int DreadlordOnBlackDragon::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void DreadlordOnBlackDragon::Init()
{

}

DreadlordOnBlackDragon::DreadlordOnBlackDragon()
{

}

bool DreadlordOnBlackDragon::configure()
{
    return false;
}

void DreadlordOnBlackDragon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}