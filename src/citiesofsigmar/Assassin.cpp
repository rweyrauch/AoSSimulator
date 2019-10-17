/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Assassin.h"

namespace CitiesOfSigmar
{

Unit *Assassin::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Assassin::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Assassin::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Assassin::Init()
{

}

Assassin::Assassin()
{

}

bool Assassin::configure()
{
    return false;
}

void Assassin::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}