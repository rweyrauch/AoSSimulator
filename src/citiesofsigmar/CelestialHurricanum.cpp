/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/CelestialHurricanum.h"

namespace CitiesOfSigmar
{

Unit *CelestialHurricanum::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string CelestialHurricanum::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int CelestialHurricanum::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void CelestialHurricanum::Init()
{

}

CelestialHurricanum::CelestialHurricanum()
{

}

bool CelestialHurricanum::configure()
{
    return false;
}

void CelestialHurricanum::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}