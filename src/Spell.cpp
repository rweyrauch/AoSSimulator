/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Spell.h>
#include <Unit.h>
#include <Board.h>
#include <MathUtils.h>

Spell::Result Spell::cast(Unit *target, int round) {
    if (target == nullptr) {
        return Result::Failed;
    }

    // Distance to target
    const double distance = m_caster->distanceTo(target);
    if (distance > (double)Dice::RollSpecial(m_range)) {
        return Result::Failed;
    }

    // Check for visibility to target
    if (m_lineOfSiteRequired && !Board::Instance()->isVisible(m_caster, target)) {
        return Result::Failed;
    }

    m_round = round;

    Spell::Result result = Result::Failed;

    int unmodifiedRoll;
    const int castingRoll = m_caster->rollCasting(unmodifiedRoll);
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            result = apply(castingRoll, unmodifiedRoll, target);
        } else {
            result = Result::Unbound;
        }
    }

    onCast(result);

    return result;
}

Spell::Result Spell::cast(double x, double y, int round) {
    const Math::Point3 targetPoint(x, y, 0.0);

    // Distance to point
    const double distance = m_caster->position().distance(targetPoint);
    if (distance > (double)Dice::RollSpecial(m_range)) {
        return Result::Failed;
    }

    m_round = round;

    Spell::Result result = Result::Failed;

    int unmodifiedRoll;
    const int castingRoll = m_caster->rollCasting(unmodifiedRoll);
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            result = apply(castingRoll, unmodifiedRoll, x, y);
        } else {
            result = Result::Unbound;
        }
    }

    onCast(result);

    return result;
}

Duration Spell::defaultDuration() const {
    return {Phase::Hero, m_round + 1, m_caster->owningPlayer()};
}

DamageSpell::DamageSpell(Unit *caster, const std::string &name, int castingValue, int range, int damage,
                         int castingValue2, int damage2) :
        Spell(caster, name, castingValue, range),
        m_damage(damage),
        m_castingValue2(castingValue2),
        m_damage2(damage2) {
    m_allowedTargets = Abilities::Target::Enemy;
    m_effect = Abilities::EffectType::Damage;
}

int DamageSpell::getDamage(int castingRoll) const {
    if ((m_castingValue2 > 0) && (castingRoll >= m_castingValue2)) {
        return m_damage2;
    }
    return m_damage;
}

Spell::Result DamageSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    auto mortalWounds = Dice::RollSpecial(getDamage(castingRoll));
    target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);
    PLOG_INFO.printf("%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
           m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, mortalWounds,
           target->name().c_str());

    return Spell::Result::Success;
}

DamageSpell *CreateArcaneBolt(Unit *caster) {
    return new DamageSpell(caster, "Arcane Bolt", 5, 18, 1, 10, RAND_D3);
}

AreaOfEffectSpell::AreaOfEffectSpell(Unit *caster, const std::string &name, int castingValue, int range, int radius,
                                     int damage, int affectedRoll) :
    Spell(caster, name, castingValue, range),
    m_damage(damage),
    m_radius(radius),
    m_affectedRoll(affectedRoll) {
    m_allowedTargets = Abilities::Target::Point;
    m_effect = Abilities::EffectType::AreaOfEffectDamage;
}


int AreaOfEffectSpell::getDamage(int /*castingRoll*/) const {
    return m_damage;
}

Spell::Result AreaOfEffectSpell::apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) {

    const Math::Point3 targetPoint(x, y, 0.0);

    const auto radius = (double)Dice::RollSpecial(m_radius);
    auto units = Board::Instance()->getUnitsWithin(targetPoint, GetEnemyId(m_caster->owningPlayer()), radius);
    for (auto target : units) {
        bool unitAffected = true;
        if (m_affectedRoll != 0) {
            int roll = Dice::RollD6();
            unitAffected = (roll >= m_affectedRoll);
        }

        if (unitAffected) {
            auto mortalWounds = Dice::RollSpecial(getDamage(castingRoll));
            target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);
            secondaryEffect(target, m_round);
            PLOG_INFO.printf("%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
                   m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, mortalWounds,
                   target->name().c_str());
        }
    }
    return Spell::Result::Success;
}

Spell::Result AreaOfEffectSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    if (target != nullptr)
        return apply(castingRoll, unmodifiedCastingRoll, target->x(), target->y());
    return Result::Failed;
}

LineOfEffectSpell::LineOfEffectSpell(Unit *caster, const std::string &name, int castingValue, int range, int damage,
                                     int affectedRoll) :
    Spell(caster, name, castingValue, range),
    m_damage(damage),
    m_affectedRoll(affectedRoll) {
    m_allowedTargets = Abilities::Target::Point;
    m_effect = Abilities::EffectType::Damage;
}

int LineOfEffectSpell::getDamage(int castingRoll) const {
    return m_damage;
}

Spell::Result LineOfEffectSpell::apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) {
    // TODO: implement
    return Spell::Result::Failed;
}

Spell::Result LineOfEffectSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    if (target != nullptr)
        return apply(castingRoll, unmodifiedCastingRoll, target->x(), target->y());
    return Spell::Result::Failed;
}

HealSpell::HealSpell(Unit *caster, const std::string &name, int castingValue, int range, int healing,
                     int castingValue2, int healing2, const std::vector<Keyword>& targetKeywords) :
    Spell(caster, name, castingValue, range),
    m_healing(healing),
    m_castingValue2(castingValue2),
    m_healing2(healing2) {
    m_allowedTargets = Abilities::Target::SelfAndFriendly;
    m_effect = Abilities::EffectType::Heal;
    m_targetKeywords = targetKeywords;
}

int HealSpell::getHealing(int castingRoll) const {
    if ((m_castingValue2 > 0) && (castingRoll >= m_castingValue2)) {
        return m_healing2;
    }
    return m_healing;
}

Spell::Result HealSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    if (target == nullptr)
        return Result::Failed;

    auto wounds = Dice::RollSpecial(getHealing(castingRoll));
    target->heal(wounds);
    PLOG_INFO.printf("%s spell %s with casting roll of %d (%d) heals %d wounds onto %s.\n",
           m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, wounds, target->name().c_str());
    return Spell::Result::Success;
}

BuffModifierSpell::BuffModifierSpell(Unit *caster, const std::string &name, int castingValue, int range,
                                     BuffableAttribute which, int modifier, Abilities::Target allowedTargets, const std::vector<Keyword>& targetKeyword) :
        Spell(caster, name, castingValue, range),
        m_attribute(which),
        m_modifier(modifier){
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = (m_modifier > 0) ? Abilities::EffectType::Buff : Abilities::EffectType::Debuff;
}

int BuffModifierSpell::getModifier(int /*castingRoll*/) const {
    return m_modifier;
}

Spell::Result BuffModifierSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    if (target == nullptr)
        return Result::Failed;

    target->buffModifier(m_attribute, getModifier(castingRoll), defaultDuration());

    return Spell::Result::Success;
}

BuffRerollSpell::BuffRerollSpell(Unit *caster, const std::string &name, int castingValue, int range,
                                 BuffableAttribute which, Rerolls reroll, Abilities::Target allowedTargets, const std::vector<Keyword>& targetKeyword) :
        Spell(caster, name, castingValue, range),
        m_attribute(which),
        m_reroll(reroll) {
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = (m_allowedTargets == Abilities::Target::Enemy) ? Abilities::EffectType::Debuff : Abilities::EffectType::Buff;
}

Spell::Result BuffRerollSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    if (target == nullptr)
        return Result::Failed;

    target->buffReroll(m_attribute, m_reroll, defaultDuration());

    return Spell::Result::Success;
}

BuffAbilitySpell::BuffAbilitySpell(Unit *caster, const std::string &name, int castingValue, int range,
                                   BuffableAbility which, int value, Abilities::Target allowedTargets,
                                   const std::vector<Keyword> &targetKeyword):
        Spell(caster, name, castingValue, range),
        m_attribute(which),
        m_value(value) {
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = (m_allowedTargets == Abilities::Target::Enemy) ? Abilities::EffectType::Debuff : Abilities::EffectType::Buff;
}

Spell::Result BuffAbilitySpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) {
    if (target == nullptr)
        return Result::Failed;

    target->buffAbility(m_attribute, m_value, defaultDuration());

    return Spell::Result::Success;
}

BuffMovementSpell::BuffMovementSpell(Unit *caster, const std::string &name, int castingValue, int range,
                                     MovementRules which, bool allowed, Abilities::Target allowedTargets,
                                     const std::vector<Keyword> &targetKeyword) :
        Spell(caster, name, castingValue, range),
        m_attribute(which),
        m_allowed(allowed) {
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = (m_allowedTargets == Abilities::Target::Enemy) ? Abilities::EffectType::Debuff : Abilities::EffectType::Buff;
}

Spell::Result BuffMovementSpell::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
    if (target == nullptr)
        return Result::Failed;

    target->buffMovement(m_attribute, m_allowed, defaultDuration());

    return Spell::Result::Success;
}
