/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Gyrobombers.h"

namespace CitiesOfSigmar
{

Unit *Gyrobombers::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Gyrobombers::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Gyrobombers::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Gyrobombers::Init()
{

}

Gyrobombers::Gyrobombers()
{

}

bool Gyrobombers::configure()
{
    return false;
}

void Gyrobombers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}