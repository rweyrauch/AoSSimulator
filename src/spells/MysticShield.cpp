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
    m_allowedTargets = Abilities::Target::Friendly;
    m_effect = Abilities::EffectType::Buff;
}

Spell::Result MysticShield::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) {
    if (target == nullptr) {
        return Result::Failed;
    }

    target->buffReroll(Attribute::To_Save_Missile, Rerolls::Ones, defaultDuration());
    target->buffReroll(Attribute::To_Save_Melee, Rerolls::Ones, defaultDuration());

    return Result::Success;
}
