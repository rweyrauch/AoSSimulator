/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/MysticShield.h>
#include <Unit.h>
#include <Board.h>

MysticShield::MysticShield(Unit *caster) :
    Spell(caster, "Mystic Shield", 6, 18) {
    m_allowedTargets = Spell::Target::Friendly;
    m_effect = EffectType::Buff;
}

Spell::Result MysticShield::apply(int castingValue, int unmodifiedCastingValue, Unit* target) {
    if (target == nullptr) {
        return Result::Failed;
    }

    if (target->buffReroll(To_Save, Reroll_Ones, {Phase::Hero, m_round + 1, m_caster->owningPlayer()})) {
        return Result::Success;
    }

    return Result::Failed;
}
