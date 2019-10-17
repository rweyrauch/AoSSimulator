/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/SistersOfTheWatch.h"

namespace CitiesOfSigmar
{

Unit *SistersOfTheWatch::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string SistersOfTheWatch::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int SistersOfTheWatch::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void SistersOfTheWatch::Init()
{

}

SistersOfTheWatch::SistersOfTheWatch()
{

}

bool SistersOfTheWatch::configure()
{
    return false;
}

void SistersOfTheWatch::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}