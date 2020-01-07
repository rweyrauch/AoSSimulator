/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/GrundstokThunderers.h>

namespace KharadronOverlords
{

Unit *GrundstokThunderers::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string GrundstokThunderers::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int GrundstokThunderers::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void GrundstokThunderers::Init()
{

}

GrundstokThunderers::GrundstokThunderers() :
    KharadronBase("Grundstok Thunderers", 4, WOUNDS, 7, 4, false)
{

}

bool GrundstokThunderers::configure()
{
    return false;
}
} //KharadronOverlords
