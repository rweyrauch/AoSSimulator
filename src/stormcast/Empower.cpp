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

Spell::Result Empower::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
    if (target == nullptr) {
        return Result::Failed;
    }

    target->buffReroll(To_Wound_Melee, Reroll_Ones, {Phase::Hero, m_round + 1, m_caster->owningPlayer()});
    target->buffReroll(To_Wound_Missile, Reroll_Ones, {Phase::Hero, m_round + 1, m_caster->owningPlayer()});

    return Spell::Result::Success;
}
