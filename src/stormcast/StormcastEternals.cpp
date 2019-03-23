/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/StormcastEternals.h>

namespace StormcastEternals
{

int StormcastEternal::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = Unit::toHitModifier(weapon, unit);

    // Beast Stalkers
    if (hasKeyword(ASTRAL_TEMPLARS) && unit->hasKeyword(MONSTER))
    {
        modifier += 1;
    }

    return modifier;
}

Rerolls StormcastEternal::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Driven by Vengeance
    if (hasKeyword(CELESTIAL_VINDICATORS) && m_charged)
    {
        return RerollOnes;
    }

    return Unit::toHitRerolls(weapon, unit);
}

int StormcastEternal::braveryModifier() const
{
    int modifier = Unit::braveryModifier();

    // First to be Forged
    if (hasKeyword(HAMMERS_OF_SIGMAR))
    {
        modifier += 1;
    }

    return modifier;
}

Rerolls StormcastEternal::battleshockRerolls() const
{
    // No True Death
    if (hasKeyword(ANVILS_OF_THE_HELDENHAMMER))
    {
        return RerollFailed;
    }

    return Unit::battleshockRerolls();
}

} // namespace StormcastEternals