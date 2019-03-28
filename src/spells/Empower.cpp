/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/Empower.h>
#include <Unit.h>
#include <Board.h>

Empower::Empower(Unit *caster) :
    Spell(caster, "Empower", 6, 12.0f)
{
    m_targetFriendly = true;
}

Spell::Result Empower::cast(Unit *target, int round)
{
    if (target == nullptr)
    {
        return Failed;
    }

    // Distance to target
    const float distance = m_caster->distanceTo(target);
    if (distance > m_range)
    {
        return Failed;
    }

    Dice dice;
    Spell::Result result = Failed;

    const int castingRoll = dice.roll2D6();
    if (castingRoll >= m_castingValue)
    {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound)
        {
            target->buffReroll(ToWound, RerollOnes, {Phase::Hero, round+1, m_caster->owningPlayer()});
            result = Success;
        }
        else
        {
            SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
            result = Unbound;
        }
    }
    else
    {
        SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(), name().c_str(),
               castingRoll, m_castingValue);
    }

    return result;
}
