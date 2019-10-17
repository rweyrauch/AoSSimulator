/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/NomadPrince.h"

namespace CitiesOfSigmar
{

Unit *NomadPrince::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string NomadPrince::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int NomadPrince::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void NomadPrince::Init()
{

}

NomadPrince::NomadPrince()
{

}

bool NomadPrince::configure()
{
    return false;
}

void NomadPrince::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}