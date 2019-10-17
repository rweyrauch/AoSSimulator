/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Dreadspears.h"

namespace CitiesOfSigmar
{

Unit *Dreadspears::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Dreadspears::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Dreadspears::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Dreadspears::Init()
{

}

Dreadspears::Dreadspears()
{

}

bool Dreadspears::configure()
{
    return false;
}

void Dreadspears::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}