/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/FleshEaterCourts.h>
#include <Board.h>

namespace FleshEaterCourt
{

Rerolls FleshEaterCourts::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if (m_delusion == TheRoyalHunt)
    {
        if (target->hasKeyword(MONSTER))
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == AMatterOfHonour)
    {
        if (target->hasKeyword(HERO))
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == TheGrandTournament)
    {
        if (!isGeneral())
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == None)
    {
        // Morgaunt Court - Blood Loyalty
        if (hasKeyword(MORGAUNT) && hasKeyword(COURTIER))
        {
            auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 12.0f);
            for (auto ip : units)
            {
                if (ip->hasKeyword(MORGAUNT) && ip->hasKeyword(SERFS))
                {
                    return RerollOnes;
                }
            }
        }
    }
    return Unit::toHitRerolls(weapon, target);
}

Rerolls FleshEaterCourts::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    if (m_delusion == TheRoyalHunt)
    {
        if (target->hasKeyword(MONSTER))
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == AMatterOfHonour)
    {
        if (target->isGeneral())
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == None)
    {
        // Hollowmourne - Shattering Charge
        if (hasKeyword(HOLLOWMOURNE) && (hasKeyword(COURTIER) || hasKeyword(KNIGHTS)))
        {
            if (charged())
            {
                return RerollOnes;
            }
        }
    }
    return Unit::toWoundRerolls(weapon, target);
}

int FleshEaterCourts::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Gristlegore - Peerless Ferocity
    if (hasKeyword(GRISTLEGORE) && (hasKeyword(HERO) || hasKeyword(MONSTER)))
    {
        if (unmodifiedHitRoll == 6)
        {
            return 2;
        }
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int FleshEaterCourts::moveModifier() const
{
    int modifier = Unit::moveModifier();
    // Blisterskin - Blistering Speed
    if (hasKeyword(BLISTERSKIN))
    {
        modifier += 2;
    }
    return modifier;
}

int FleshEaterCourts::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_delusion == CrusadingArmy)
    {
        modifier += 1;
    }
    return modifier;
}

int FleshEaterCourts::chargeModifier() const
{
    int modifier = Unit::chargeModifier();
    if (m_delusion == CrusadingArmy)
    {
        modifier += 1;
    }
    return modifier;
}

Wounds FleshEaterCourts::applyWoundSave(const Wounds &wounds)
{
    Dice dice;
    // Deathless Courtiers
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 12.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(FLESH_EATER_COURTS) && ip->hasKeyword(HERO))
        {
            Dice::RollResult woundSaves, mortalSaves;
            dice.rollD6(wounds.normal, woundSaves);
            dice.rollD6(wounds.mortal, mortalSaves);

            Wounds totalWounds = wounds;
            totalWounds.normal -= woundSaves.rollsGE(6);
            totalWounds.normal = std::max(totalWounds.normal, 0);
            totalWounds.mortal -= mortalSaves.rollsGE(6);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);

            return totalWounds;
        }
    }

    return Unit::applyWoundSave(wounds);
}

} // namespace FleshEaterCourt
