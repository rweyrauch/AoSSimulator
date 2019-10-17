/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/BlackGuard.h"

namespace CitiesOfSigmar
{

Unit *BlackGuard::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string BlackGuard::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int BlackGuard::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void BlackGuard::Init()
{

}

BlackGuard::BlackGuard()
{

}

bool BlackGuard::configure()
{
    return false;
}

void BlackGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}