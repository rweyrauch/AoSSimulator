/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/GrundstokGunhauler.h>

namespace KharadronOverlords
{

Unit *GrundstokGunhauler::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string GrundstokGunhauler::ValueToString(const Parameter &parameter)
{
    return KharadronBase::ValueToString(parameter);
}

int GrundstokGunhauler::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void GrundstokGunhauler::Init()
{

}

GrundstokGunhauler::GrundstokGunhauler() :
    KharadronBase("Grundstok Gunhauler", 12, WOUNDS, 7, 4, true)
{

}

bool GrundstokGunhauler::configure()
{
    return false;
}
} //KharadronOverlords
