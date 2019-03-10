/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/ArcaneBolt.h>

ArcaneBolt::ArcaneBolt(Unit *caster) :
    DamageSpell(caster, "Arcane Bolt", 5, 18.0f, 1)
{}

int ArcaneBolt::getDamage(int castingRoll) const
{
    if (castingRoll >= 10)
    {
        return RAND_D3;
    }
    return m_damage;
}
