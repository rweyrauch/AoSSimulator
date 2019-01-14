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

DamageSpell::DamageSpell(Unit* caster, const std::string& name, int castingValue, int range, int damage) :
    Spell(caster, name, castingValue),
    m_range(range),
    m_damage(damage) {}

int DamageSpell::cast(const Unit* target)
{
    if (target == nullptr)
        return 0;

    // Distance to target
    const float distance = m_caster->distanceTo(target);
    if (distance > m_range)
        return 0;

    Dice dice;

    const int castingRoll = dice.rollD6();
    if (castingRoll >= m_castingValue)
    {
        // TODO: Implement unbinding roll
        //bool unbound = otherplayer->unbind(m_caster, castingRoll);

        const int mortalWounds = dice.rollSpecial(m_damage);
        return mortalWounds;
    }

    return 0;
}

ArcaneBolt::ArcaneBolt(Unit *caster) :
    DamageSpell(caster, "Arcane Bolt", 5, 18, 1)
{}