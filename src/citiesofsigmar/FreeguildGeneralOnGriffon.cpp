/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"

namespace CitiesOfSigmar
{

Unit *FreeguildGeneralOnGriffon::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildGeneralOnGriffon::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildGeneralOnGriffon::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildGeneralOnGriffon::Init()
{

}

FreeguildGeneralOnGriffon::FreeguildGeneralOnGriffon()
{

}

bool FreeguildGeneralOnGriffon::configure()
{
    return false;
}

void FreeguildGeneralOnGriffon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}