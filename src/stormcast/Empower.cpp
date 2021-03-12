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
    m_allowedTargets = Abilities::Target::SelfAndFriendly;
    m_effect = Abilities::EffectType::Buff;
}

Spell::Result Empower::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
    if (target == nullptr) {
        return Result::Failed;
    }

    target->buffReroll(Attribute::To_Wound_Melee, Rerolls::Ones, defaultDuration());
    target->buffReroll(Attribute::To_Wound_Missile, Rerolls::Ones, defaultDuration());

    return Spell::Result::Success;
}
