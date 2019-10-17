/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/BlackArkCorsairs.h"

namespace CitiesOfSigmar
{

Unit *BlackArkCorsairs::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string BlackArkCorsairs::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int BlackArkCorsairs::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void BlackArkCorsairs::Init()
{

}

BlackArkCorsairs::BlackArkCorsairs()
{

}

bool BlackArkCorsairs::configure()
{
    return false;
}

void BlackArkCorsairs::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}