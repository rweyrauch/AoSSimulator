/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Cogsmith.h"

namespace CitiesOfSigmar
{

Unit *Cogsmith::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Cogsmith::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Cogsmith::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Cogsmith::Init()
{

}

Cogsmith::Cogsmith()
{

}

bool Cogsmith::configure()
{
    return false;
}

void Cogsmith::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}