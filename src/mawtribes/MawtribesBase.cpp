/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "mawtribes/MawtribesBase.h"

#include "mawtribes/Tyrant.h"
#include "mawtribes/Ironblaster.h"
#include "mawtribes/Leadbelchers.h"
#include "mawtribes/OgorGluttons.h"

namespace OgorMawtribes
{

std::string MawtribesBase::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int MawtribesBase::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Init()
{
    Tyrant::Init();
    Ironblaster::Init();
    Leadbelchers::Init();
    OgorGluttons::Init();
}

} // namespace OgorMawtribes
