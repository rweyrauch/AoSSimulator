/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/SteamTank.h"

namespace CitiesOfSigmar
{

Unit *SteamTank::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string SteamTank::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int SteamTank::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void SteamTank::Init()
{

}

SteamTank::SteamTank()
{

}

bool SteamTank::configure()
{
    return false;
}

void SteamTank::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}