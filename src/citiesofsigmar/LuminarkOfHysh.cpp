/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/LuminarkOfHysh.h"

namespace CitiesOfSigmar
{

Unit *LuminarkOfHysh::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string LuminarkOfHysh::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int LuminarkOfHysh::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void LuminarkOfHysh::Init()
{

}

LuminarkOfHysh::LuminarkOfHysh()
{

}

bool LuminarkOfHysh::configure()
{
    return false;
}

void LuminarkOfHysh::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}