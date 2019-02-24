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

DamageSpell::DamageSpell(Unit *caster, const std::string &name, int castingValue, int range, int damage) :
    Spell(caster, name, castingValue),
    m_range(range),
    m_damage(damage)
{}

int DamageSpell::cast(const Unit *target)
{
    if (target == nullptr)
    {
        return 0;
    }

    // Distance to target
    const float distance = m_caster->distanceTo(target);
    if (distance > m_range)
    {
        return 0;
    }

    Dice dice;

    int mortalWounds = 0;
    const int castingRoll = dice.rollD6();
    if (castingRoll >= m_castingValue)
    {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound)
        {
            mortalWounds = dice.rollSpecial(getDamage(castingRoll));
        }
    }

    return mortalWounds;
}

ArcaneBolt::ArcaneBolt(Unit *caster) :
    DamageSpell(caster, "Arcane Bolt", 5, 18, 1)
{}

int ArcaneBolt::getDamage(int castingRoll) const
{
    if (castingRoll >= 10)
    {
        return RAND_D3;
    }
    return m_damage;
}
