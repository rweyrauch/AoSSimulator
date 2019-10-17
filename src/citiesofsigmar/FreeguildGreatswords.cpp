/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "citiesofsigmar/FreeguildGreatswords.h"

namespace CitiesOfSigmar
{

Unit *FreeguildGreatswords::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string FreeguildGreatswords::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int FreeguildGreatswords::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void FreeguildGreatswords::Init()
{

}

FreeguildGreatswords::FreeguildGreatswords()
{

}

bool FreeguildGreatswords::configure()
{
    return false;
}

void FreeguildGreatswords::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}
}