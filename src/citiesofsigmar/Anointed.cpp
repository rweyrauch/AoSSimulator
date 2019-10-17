/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Anointed.h"

namespace CitiesOfSigmar
{

Unit *Anointed::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Anointed::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Anointed::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Anointed::Init()
{

}

Anointed::Anointed()
{

}

bool Anointed::configure()
{
    return false;
}

void Anointed::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}