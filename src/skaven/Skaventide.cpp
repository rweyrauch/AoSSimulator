/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Skaventide.h>
#include <UnitFactory.h>

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

} // namespace Skaven