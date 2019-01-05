/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <Weapon.h>

#include "WarhammerSim.h"
#include "Weapon.h"
#include "Dice.h"

Weapon::Weapon(const std::string& name, int range, int attacks, int toHit, int toWound, int rend, int damage) :
    m_name(name),
    m_range(range),
    m_attacks(attacks),
    m_toHit(toHit),
    m_toWound(toWound),
    m_rend(rend),
    m_damage(damage)
{
}

int Weapon::rollToHit() const
{
    Dice dice;
    auto rolls = dice.rollD6(numAttacks());
    const int toHit = m_toHit;
    auto hitCheck = [toHit](int v) { return v >= toHit; };
    return (int)std::count_if(rolls.begin(), rolls.end(), hitCheck);
}

int Weapon::rollToWound(int numHits) const
{
    Dice dice;
    int totalHits = numHits;
    if (m_hitsPerAttack > 1)
    {
        for (int i = 0; i < numHits; i++)
        {
            totalHits += numTotalHits();
        }
    }

    auto rolls = dice.rollD6(totalHits);
    const int toWound = m_toWound;
    auto woundCheck = [toWound](int v)
    { return v >= toWound; };
    return (int) std::count_if(rolls.begin(), rolls.end(), woundCheck);
}

int Weapon::numAttacks() const
{
    return rollSpecial(m_attacks);
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
