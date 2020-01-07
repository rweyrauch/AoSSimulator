/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/Endrinriggers.h>

namespace KharadronOverlords
{

Unit *Endrinriggers::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Endrinriggers::ValueToString(const Parameter &parameter)
{
    return KharadronBase::ValueToString(parameter);
}

int Endrinriggers::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Endrinriggers::Init()
{

}

Endrinriggers::Endrinriggers() :
    KharadronBase("Endrinriggers", 12, WOUNDS, 7, 4, true)
{

}

bool Endrinriggers::configure()
{
    return false;
}
} //KharadronOverlords
