/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FlamespyrePhoenix.h"

namespace CitiesOfSigmar
{

Unit *FlamespyrePhoenix::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FlamespyrePhoenix::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FlamespyrePhoenix::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FlamespyrePhoenix::Init()
{

}

FlamespyrePhoenix::FlamespyrePhoenix()
{

}

bool FlamespyrePhoenix::configure()
{
    return false;
}

void FlamespyrePhoenix::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}