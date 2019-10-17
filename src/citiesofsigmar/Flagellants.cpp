/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Flagellants.h"

namespace CitiesOfSigmar
{

Unit *Flagellants::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Flagellants::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Flagellants::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Flagellants::Init()
{

}

Flagellants::Flagellants()
{

}

bool Flagellants::configure()
{
    return false;
}

void Flagellants::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}