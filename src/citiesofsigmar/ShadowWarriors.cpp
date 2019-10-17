/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/ShadowWarriors.h"

namespace CitiesOfSigmar
{

Unit *ShadowWarriors::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string ShadowWarriors::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int ShadowWarriors::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void ShadowWarriors::Init()
{

}

ShadowWarriors::ShadowWarriors()
{

}

bool ShadowWarriors::configure()
{
    return false;
}

void ShadowWarriors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}