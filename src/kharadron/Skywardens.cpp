/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/Skywardens.h>

namespace KharadronOverlords
{

Unit *Skywardens::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string Skywardens::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Skywardens::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Skywardens::Init()
{

}

Skywardens::Skywardens() :
    KharadronBase("Skywardens", 4, WOUNDS, 6, 4, false)
{

}

bool Skywardens::configure()
{
    return false;
}

} //KharadronOverlords
