/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautIronclad.h>

namespace KharadronOverlords
{

Unit *ArkanautIronclad::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string ArkanautIronclad::ValueToString(const Parameter &parameter)
{
    return KharadronBase::ValueToString(parameter);
}

int ArkanautIronclad::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void ArkanautIronclad::Init()
{

}

ArkanautIronclad::ArkanautIronclad() :
    KharadronBase("Arkanaut Ironclad", 10, WOUNDS, 8, 3, true)
{

}

bool ArkanautIronclad::configure()
{
    return false;
}
} //KharadronOverlords
