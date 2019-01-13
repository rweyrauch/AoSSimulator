/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <Weapon.h>
#include <WarhammerSim.h>
#include <Dice.h>

Weapon::Weapon(Type type, const std::string& name, int range, int attacks, int toHit, int toWound, int rend, int damage) :
    m_type(type),
    m_name(name),
    m_range(range),
    m_attacks(attacks),
    m_toHit(toHit),
    m_toWound(toWound),
    m_rend(rend),
    m_damage(damage)
{
}
Weapon::Weapon(const Weapon& w) :
    m_type(w.m_type),
    m_name(w.m_name),
    m_range(w.m_range),
    m_attacks(w.m_attacks),
    m_toHit(w.m_toHit),
    m_toWound(w.m_toWound),
    m_rend(w.m_rend),
    m_damage(w.m_damage)
{
}

Hits Weapon::rollToHit(int modifier, Rerolls rerolls, int extraAttacks) const
{
    Dice dice;
    Dice::RollResult rollResult;

    const int totalAttacks = numAttacks(extraAttacks);

    const int toHit = m_toHit - modifier;

    int numHits = 0;
    if (rerolls == RerollOnes)
    {
        dice.rollD6(totalAttacks, 1, rollResult);
        numHits = rollResult.rollsGE(toHit);
    }
    else if (rerolls == RerollFailed)
    {
        dice.rollD6(totalAttacks, rollResult);
        numHits = rollResult.rollsGE(toHit);
        int numFails = totalAttacks - numHits;
        if (numFails > 0)
        {
            rollResult.clearLT(toHit);
            Dice::RollResult rerollResult;
            dice.rollD6(numFails, rerollResult);
            auto numRerolledHits = rerollResult.rollsGE(toHit);
            numHits += numRerolledHits;
            // merge roll results from rerolls into a single result.
            rollResult.add(rerollResult);
        }
    }
    else
    {
        dice.rollD6(totalAttacks, rollResult);
        numHits = rollResult.rollsGE(toHit);
    }
    return {numHits, rollResult};
}

WoundingHits Weapon::rollToWound(int numHits, int modifier, Rerolls rerolls) const
{
    Dice dice;
    Dice::RollResult rollResult;

    int totalHits = numHits;
    if (m_hitsPerAttack < 0)
    {
        totalHits = 0;
        for (int i = 0; i < numHits; i++)
        {
            totalHits += numTotalHits();
        }
    }
    const int toWound = m_toWound - modifier;

    int numWoundingHits = 0;

    if (rerolls == RerollOnes)
    {
        dice.rollD6(totalHits, 1, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
    }
    else if (rerolls == RerollFailed)
    {
        dice.rollD6(totalHits, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
        int numFails = totalHits - numWoundingHits;
        if (numFails > 0)
        {
            rollResult.clearLT(toWound);
            Dice::RollResult rerollResult;
            dice.rollD6(numFails, rerollResult);
            auto numRerolledHits = rerollResult.rollsGE(toWound);
            numWoundingHits += numRerolledHits;
            // merge roll results from rerolls into a single result.
            rollResult.add(rerollResult);
        }
    }
    else
    {
        dice.rollD6(totalHits, rollResult);
        numWoundingHits = rollResult.rollsGE(toWound);
    }

    return {numWoundingHits, rollResult};
}

int Weapon::numAttacks(int extraAttacks) const
{
    return rollSpecial(m_attacks) + extraAttacks;
}

int Weapon::damage() const
{
    return rollSpecial(m_damage);
}

int Weapon::rollSpecial(int number) const
{
    Dice dice;
    if (number >= 0)
        return number;
    else if (number == RAND_D3)
        return dice.rollD3();
    else if (number == RAND_D6)
        return dice.rollD6();
    else if (number == RAND_2D6)
        return dice.roll2D6();
    else if (number == RAND_3D6)
        return dice.roll3D6();
    else if (number == RAND_4D6)
        return dice.roll4D6();
    return 0;
}

int Weapon::numTotalHits() const
{
    return rollSpecial(m_hitsPerAttack);
}
