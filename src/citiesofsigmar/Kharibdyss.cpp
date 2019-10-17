/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Kharibdyss.h"

namespace CitiesOfSigmar
{

Unit *Kharibdyss::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Kharibdyss::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Kharibdyss::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Kharibdyss::Init()
{

}

Kharibdyss::Kharibdyss()
{

}

bool Kharibdyss::configure()
{
    return false;
}

void Kharibdyss::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}