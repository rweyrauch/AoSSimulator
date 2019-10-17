/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/ScourgerunnerChariots.h"

namespace CitiesOfSigmar
{

Unit *ScourgerunnerChariots::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string ScourgerunnerChariots::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int ScourgerunnerChariots::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void ScourgerunnerChariots::Init()
{

}

ScourgerunnerChariots::ScourgerunnerChariots()
{

}

bool ScourgerunnerChariots::configure()
{
    return false;
}

void ScourgerunnerChariots::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}