/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/HelblasterVolleyGun.h"

namespace CitiesOfSigmar
{

Unit *HelblasterVolleyGun::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string HelblasterVolleyGun::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int HelblasterVolleyGun::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void HelblasterVolleyGun::Init()
{

}

HelblasterVolleyGun::HelblasterVolleyGun()
{

}

bool HelblasterVolleyGun::configure()
{
    return false;
}

void HelblasterVolleyGun::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}