/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/Nighthaunt.h>
#include <Board.h>

#include "nighthaunt/ChainraspHorde.h"
#include "nighthaunt/GrimghastReapers.h"
#include "nighthaunt/BladegheistRevenants.h"
#include "nighthaunt/GlaivewraithStalkers.h"
#include "nighthaunt/DreadscytheHarridans.h"
#include "nighthaunt/SpiritHosts.h"
#include "nighthaunt/Hexwraiths.h"
#include "nighthaunt/Chainghasts.h"

namespace Nighthaunt
{

Wounds Nighthaunt::applyWoundSave(const Wounds &wounds)
{
    // Deathless Spirits
    auto hero = Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, HERO, 12.0f);
    if (hero && hero->hasKeyword(NIGHTHAUNT))
    {
        Dice dice;
        Dice::RollResult woundSaves, mortalSaves;
        dice.rollD6(wounds.normal, woundSaves);
        dice.rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }
    return Unit::applyWoundSave(wounds);
}


int Nighthaunt::toSaveModifier(const Weapon *weapon) const
{
    // Ethereal - no save modifiers allowed.
    int modifier = 0;

    // Ethereal - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

void Init()
{
    ChainraspHorde::Init();
    GrimghastReapers::Init();
    BladegheistRevenants::Init();
    GlaivewraithStalkers::Init();
    DreadscytheHarridans::Init();
    Hexwraiths::Init();
    SpiritHosts::Init();
    Chainghasts::Init();
}

} // namespace Nighthaunt
