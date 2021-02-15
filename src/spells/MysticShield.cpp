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
        Spell(caster, "Mystic Shield", 6, 18.0) {
    m_allowedTargets = Spell::Target::Friendly;
    m_effect = EffectType::Buff;
}

Spell::Result MysticShield::cast(Unit *target, int round) {
    if (target == nullptr) {
        return Result::Failed;
    }

    // Distance to target
    const double distance = m_caster->distanceTo(target);
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
            target->buffReroll(To_Save, Reroll_Ones, {Phase::Hero, round + 1, m_caster->owningPlayer()});
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
