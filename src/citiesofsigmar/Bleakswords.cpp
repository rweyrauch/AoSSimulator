/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Bleakswords.h"

namespace CitiesOfSigmar
{

Unit *Bleakswords::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Bleakswords::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Bleakswords::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Bleakswords::Init()
{

}

Bleakswords::Bleakswords()
{

}

bool Bleakswords::configure()
{
    return false;
}

void Bleakswords::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}