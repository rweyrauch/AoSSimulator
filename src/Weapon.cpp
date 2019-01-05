/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
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
    auto rolls = dice.rollD6(m_attacks);
    const int toHit = m_toHit;
    auto hitCheck = [toHit](int v) { return v >= toHit; };
    return (int)std::count_if(rolls.begin(), rolls.end(), hitCheck);
}

int Weapon::rollToWound(int numHits) const
{
    Dice dice;
    auto rolls = dice.rollD6(numHits);
    const int toWound = m_toWound;
    auto woundCheck = [toWound](int v) { return v >= toWound; };
    return (int)std::count_if(rolls.begin(), rolls.end(), woundCheck);
}
