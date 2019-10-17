/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/Gyrocopters.h"

namespace CitiesOfSigmar
{

Unit *Gyrocopters::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Gyrocopters::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Gyrocopters::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Gyrocopters::Init()
{

}

Gyrocopters::Gyrocopters()
{

}

bool Gyrocopters::configure()
{
    return false;
}

void Gyrocopters::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}