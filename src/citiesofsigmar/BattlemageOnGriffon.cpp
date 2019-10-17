/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/BattlemageOnGriffon.h"

namespace CitiesOfSigmar
{

Unit *BattlemageOnGriffon::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string BattlemageOnGriffon::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int BattlemageOnGriffon::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void BattlemageOnGriffon::Init()
{

}

BattlemageOnGriffon::BattlemageOnGriffon()
{

}

bool BattlemageOnGriffon::configure()
{
    return false;
}

void BattlemageOnGriffon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}