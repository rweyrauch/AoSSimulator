/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Darkshards.h"

namespace CitiesOfSigmar
{

Unit *Darkshards::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Darkshards::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Darkshards::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Darkshards::Init()
{

}

Darkshards::Darkshards()
{

}

bool Darkshards::configure()
{
    return false;
}

void Darkshards::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}