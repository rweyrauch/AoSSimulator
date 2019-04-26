/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include <Unit.h>
#include "Prayer.h"

DamagePrayer::DamagePrayer(Unit *source, const std::string &name, int prayingValue, float range, int damage) :
    Prayer(source, name, prayingValue, range),
    m_damage(damage)
{
    m_targetFriendly = false;
}

bool DamagePrayer::pray(Unit *target, int round)
{
    if (target == nullptr)
    {
        return false;
    }

    // Distance to target
    const float distance = m_priest->distanceTo(target);
    if (distance > m_range)
    {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target))
    {
        return false;
    }

    Dice dice;
    bool result = false;

    int mortalWounds = 0;
    const int prayerRoll = dice.roll2D6();
    if (prayerRoll >= m_prayingValue)
    {
        mortalWounds = dice.rollSpecial(getDamage(prayerRoll));
        target->applyDamage({0, mortalWounds});
        SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) inflicts %d mortal wounds into %s.\n",
               m_priest->name().c_str(), name().c_str(), prayerRoll, m_prayingValue, mortalWounds, target->name().c_str());
        result = true;
    }

    return result;
}

int DamagePrayer::getDamage(int castingRoll) const
{
    return m_damage;
}

HealPrayer::HealPrayer(Unit* source, const std::string& name, int prayingValue, float range, int healing) :
    Prayer(source, name, prayingValue, range),
    m_healing(healing)
{
    m_targetFriendly = true;
}

bool HealPrayer::pray(Unit *target, int round)
{
    if (target == nullptr)
    {
        return false;
    }

    // Distance to target
    const float distance = m_priest->distanceTo(target);
    if (distance > m_range)
    {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target))
    {
        return false;
    }

    Dice dice;

    bool result = false;

    int wounds = 0;
    const int prayingRoll = dice.roll2D6();
    if (prayingRoll >= m_prayingValue)
    {
        wounds = dice.rollSpecial(getHealing(prayingRoll));
        target->heal(wounds);
        SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) heals %d wounds onto %s.\n",
               m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, wounds, target->name().c_str());
        result = true;
    }

    return result;
}
