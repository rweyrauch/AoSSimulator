/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Battlemage.h"

namespace CitiesOfSigmar
{

Unit *Battlemage::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Battlemage::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Battlemage::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Battlemage::Init()
{

}

Battlemage::Battlemage()
{

}

bool Battlemage::configure()
{
    return false;
}

void Battlemage::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}