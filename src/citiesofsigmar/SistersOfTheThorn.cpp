/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/SistersOfTheThorn.h"

namespace CitiesOfSigmar
{

Unit *SistersOfTheThorn::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string SistersOfTheThorn::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int SistersOfTheThorn::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void SistersOfTheThorn::Init()
{

}

SistersOfTheThorn::SistersOfTheThorn()
{

}

bool SistersOfTheThorn::configure()
{
    return false;
}

void SistersOfTheThorn::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}