/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/BlackArkFleetmaster.h"

namespace CitiesOfSigmar
{

Unit *BlackArkFleetmaster::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string BlackArkFleetmaster::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int BlackArkFleetmaster::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void BlackArkFleetmaster::Init()
{

}

BlackArkFleetmaster::BlackArkFleetmaster()
{

}

bool BlackArkFleetmaster::configure()
{
    return false;
}

void BlackArkFleetmaster::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}