/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/SorceressOnBlackDragon.h"

namespace CitiesOfSigmar
{

Unit *SorceressOnBlackDragon::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string SorceressOnBlackDragon::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int SorceressOnBlackDragon::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void SorceressOnBlackDragon::Init()
{

}

SorceressOnBlackDragon::SorceressOnBlackDragon()
{

}

bool SorceressOnBlackDragon::configure()
{
    return false;
}

void SorceressOnBlackDragon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}