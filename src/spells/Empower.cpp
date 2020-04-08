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
        Spell(caster, "Empower", 6, 12.0) {
    m_targetFriendly = true;
}

Spell::Result Empower::cast(Unit *target, int round) {
    if (target == nullptr) {
        return Failed;
    }

    // Distance to target
    const auto distance = m_caster->distanceTo(target);
    if (distance > m_range) {
        return Failed;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_caster, target)) {
        return Failed;
    }

    Spell::Result result = Failed;

    const int castingRoll = m_caster->rollCasting();
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            target->buffReroll(ToWoundMelee, RerollOnes, {Phase::Hero, round + 1, m_caster->owningPlayer()});
            target->buffReroll(ToWoundMissile, RerollOnes, {Phase::Hero, round + 1, m_caster->owningPlayer()});
            result = Success;
        } else {
            SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
            result = Unbound;
        }
    } else {
        SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(),
               name().c_str(),
               castingRoll, m_castingValue);
    }

    return result;
}
