/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Skaventide.h>
#include <UnitFactory.h>

#include "skaven/Clanrats.h"
#include "skaven/GutterRunners.h"
#include "skaven/PlagueMonks.h"
#include "skaven/NightRunners.h"
#include "skaven/Stormfiends.h"
#include "skaven/Stormvermin.h"
#include "skaven/HellPitAbomination.h"
#include "skaven/RatlingGun.h"
#include "skaven/WarpfireThrower.h"
#include "skaven/WarpLightningCannon.h"
#include "skaven/Doomwheel.h"
#include "skaven/Doomflayer.h"

namespace Skaven
{

int Skaventide::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Overwhelming Mass
    if (!weapon->isMissile() && remainingModels() >= 20)
        modifier += 1;

    return modifier;
}

int Skaventide::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toWoundModifier(weapon, target);

    // Overwhelming Mass
    if (!weapon->isMissile() && remainingModels() >= 30)
        modifier += 1;

    return modifier;
}

int Skaventide::braveryModifier() const
{
    int modifier = Unit::braveryModifier();

    // Strength in Numbers
    modifier += remainingModels() / 10;

    return modifier;
}

void Init()
{
    Clanrats::Init();
    GutterRunners::Init();
    NightRunners::Init();
    PlagueMonks::Init();
    Stormfiends::Init();
    Stormvermin::Init();
    HellPitAbomination::Init();
    RatlingGun::Init();
    WarpfireThrower::Init();
    WarpLightningCannon::Init();
    Doomwheel::Init();
    Doomflayer::Init();
}

} // namespace Skaven