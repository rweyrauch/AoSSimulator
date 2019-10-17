/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Sorceress.h"

namespace CitiesOfSigmar
{

Unit *Sorceress::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Sorceress::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Sorceress::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Sorceress::Init()
{

}

Sorceress::Sorceress()
{

}

bool Sorceress::configure()
{
    return false;
}

void Sorceress::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}