/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautFrigate.h>

namespace KharadronOverlords
{

Unit *ArkanautFrigate::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string ArkanautFrigate::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int ArkanautFrigate::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void ArkanautFrigate::Init()
{

}

ArkanautFrigate::ArkanautFrigate() :
    KharadronBase("Arkanaut Frigate", 12, WOUNDS, 8, 4, true)
{

}

bool ArkanautFrigate::configure()
{
    return false;
}
} //KharadronOverlords
