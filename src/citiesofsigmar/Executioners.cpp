/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Executioners.h>

#include "citiesofsigmar/EternalGuard.h"

namespace CitiesOfSigmar
{

Unit *CitiesOfSigmar::Executioners::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Executioners::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Executioners::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Executioners::Init()
{

}

Executioners::Executioners()
{

}

bool Executioners::configure()
{
    return false;
}

void Executioners::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

}