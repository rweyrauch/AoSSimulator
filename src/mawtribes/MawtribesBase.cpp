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
#include "mawtribes/Butcher.h"
#include "mawtribes/Slaughtermaster.h"
#include "mawtribes/Gorgers.h"
#include "mawtribes/Ironguts.h"
#include "mawtribes/Scraplauncher.h"
#include "mawtribes/Gnoblars.h"
#include "mawtribes/Firebelly.h"
#include "mawtribes/Maneaters.h"
#include "mawtribes/FrostlordStonehorn.h"
#include "mawtribes/FrostlordThundertusk.h"
#include "mawtribes/HuskardStonehorn.h"
#include "mawtribes/HuskardThundertusk.h"
#include "mawtribes/StonehornBeastriders.h"
#include "mawtribes/ThundertuskBeastriders.h"

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
    Butcher::Init();
    Firebelly::Init();
    Gnoblars::Init();
    Gorgers::Init();
    Ironblaster::Init();
    Ironguts::Init();
    Leadbelchers::Init();
    Maneaters::Init();
    OgorGluttons::Init();
    GnoblarScraplauncher::Init();
    Slaughtermaster::Init();
    Tyrant::Init();
    FrostlordOnStonehorn::Init();
    FrostlordOnThundertusk::Init();
    HuskardOnStonehorn::Init();
    HuskardOnThundertusk::Init();
    StonehornBeastriders::Init();
    ThundertuskBeastriders::Init();
}

} // namespace OgorMawtribes
