/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/DemigryphKnights.h"

namespace CitiesOfSigmar
{

Unit *DemigryphKnights::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string DemigryphKnights::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int DemigryphKnights::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void DemigryphKnights::Init()
{

}

DemigryphKnights::DemigryphKnights()
{

}

bool DemigryphKnights::configure()
{
    return false;
}

void DemigryphKnights::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}