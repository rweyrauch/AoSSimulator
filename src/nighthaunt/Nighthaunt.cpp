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
#include "nighthaunt/GuardianOfSouls.h"
#include "nighthaunt/KnightOfShrouds.h"
#include "nighthaunt/KnightOfShroudsOnSteed.h"
#include "nighthaunt/KurdossValentian.h"
#include "nighthaunt/ReikenorGrimhailer.h"
#include "nighthaunt/SpiritTorment.h"
#include "nighthaunt/DreadbladeHarrow.h"
#include "nighthaunt/LadyOlynder.h"
#include "nighthaunt/LordExecutioner.h"
#include "nighthaunt/TombBanshee.h"
#include "nighthaunt/BlackCoach.h"


namespace Nighthaunt
{

Wounds Nighthaunt::applyWoundSave(const Wounds &wounds)
{
    // Deathless Spirits
    auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0f);
    if (hero && hero->hasKeyword(NIGHTHAUNT))
    {
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

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
    GuardianOfSouls::Init();
    KnightOfShrouds::Init();
    KnightOfShroudsOnEtherealSteed::Init();
    KurdossValentian::Init();
    ReikenorTheGrimhailer::Init();
    SpiritTorment::Init();
    DreadbladeHarrow::Init();
    LadyOlynder::Init();
    LordExecutioner::Init();
    TombBanshee::Init();
    BlackCoach::Init();
}

} // namespace Nighthaunt
