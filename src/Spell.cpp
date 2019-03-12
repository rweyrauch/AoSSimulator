/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Spell.h>
#include <Unit.h>
#include <Board.h>

DamageSpell::DamageSpell(Unit *caster, const std::string &name, int castingValue, float range, int damage) :
    Spell(caster, name, castingValue, range),
    m_damage(damage)
{
    m_targetFriendly = false;
}

bool DamageSpell::cast(Unit *target)
{
    if (target == nullptr)
    {
        return false;
    }

    // Distance to target
    const float distance = m_caster->distanceTo(target);
    if (distance > m_range)
    {
        return false;
    }

    Dice dice;

    int mortalWounds = 0;
    const int castingRoll = dice.roll2D6();
    if (castingRoll >= m_castingValue)
    {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound)
        {
            mortalWounds = dice.rollSpecial(getDamage(castingRoll));
            target->applyDamage({0, mortalWounds});
            SimLog(Verbosity::Narrative, "%s spell %s inflicts %d mortal wounds into %s.\n", m_caster->name().c_str(), name().c_str(),
                mortalWounds, target->name().c_str());
        }
    }

    return true;
}
