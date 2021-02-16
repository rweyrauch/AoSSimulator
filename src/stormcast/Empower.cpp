/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/Empower.h>
#include <Unit.h>
#include <Board.h>

Empower::Empower(Unit *caster) :
        Spell(caster, "Empower", 6, 12) {
    m_allowedTargets = Spell::Target::SelfAndFriendly;
    m_effect = EffectType::Buff;
}

Spell::Result Empower::cast(Unit *target, int round) {
    if (target == nullptr) {
        return Result::Failed;
    }

    // Distance to target
    const auto distance = m_caster->distanceTo(target);
    if (distance > m_range) {
        return Result::Failed;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_caster, target)) {
        return Result::Failed;
    }

    Spell::Result result = Result::Failed;

    const int castingRoll = m_caster->rollCasting();
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            target->buffReroll(To_Wound_Melee, Reroll_Ones, {Phase::Hero, round + 1, m_caster->owningPlayer()});
            target->buffReroll(To_Wound_Missile, Reroll_Ones, {Phase::Hero, round + 1, m_caster->owningPlayer()});
            result = Result::Success;
        } else {
            SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
            result = Result::Unbound;
        }
    } else {
        SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(),
               name().c_str(),
               castingRoll, m_castingValue);
    }

    return result;
}
