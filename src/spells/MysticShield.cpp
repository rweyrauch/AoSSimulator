/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/MysticShield.h>
#include <Unit.h>
#include <Board.h>

MysticShield::MysticShield(Unit *caster) :
    Spell(caster, "Mystic Shield", 6, 18.0f)
{
    m_targetFriendly = true;
}

bool MysticShield::cast(Unit *target)
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
    bool successful = false;

    const int castingRoll = dice.roll2D6();
    if (castingRoll >= m_castingValue)
    {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound)
        {
            target->buffReroll(ToSave, RerollOnes, m_duration);
            successful = true;
        }
        else
        {
            SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
        }
    }
    else
    {
        SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(), name().c_str(),
            castingRoll, m_castingValue);
    }

    return successful;
}
