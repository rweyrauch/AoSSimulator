/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/HelstormRocketBattery.h"

namespace CitiesOfSigmar
{


Unit *HelstormRocketBattery::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string HelstormRocketBattery::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int HelstormRocketBattery::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void HelstormRocketBattery::Init()
{

}

HelstormRocketBattery::HelstormRocketBattery()
{

}

bool HelstormRocketBattery::configure()
{
    return false;
}

void HelstormRocketBattery::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

}
