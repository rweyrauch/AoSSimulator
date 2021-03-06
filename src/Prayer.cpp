/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include <Unit.h>
#include "Prayer.h"

bool Prayer::pray(Unit *target, int round) {
    if (target == nullptr) {
        return false;
    }

    // Distance to target
    const double distance = m_priest->distanceTo(target);
    if (distance > m_range) {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target)) {
        return false;
    }

    bool result = false;

    const auto reroll = m_priest->prayerRerolls();
    const auto mod = m_priest->prayerModifier();

    int prayingRoll = Dice::RollD6() + mod;
    if ((prayingRoll == 1) && (reroll == Rerolls::Ones)) {
        prayingRoll = Dice::RollD6() + mod;
    } else if ((prayingRoll < m_prayingValue) && (reroll == Rerolls::Failed)) {
        prayingRoll = Dice::RollD6() + mod;
    }
    if (prayingRoll >= m_prayingValue) {
        result = apply(prayingRoll, target);
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1, Wounds::Source::Prayer, this}, m_priest);
    }

    return result;
}

bool Prayer::pray(double x, double y, int round) {
    const Math::Point3 targetPoint(x, y, 0.0);

    // Distance to point
    const double distance = m_priest->position().distance(targetPoint);
    if (distance > (double) Dice::RollSpecial(m_range)) {
        return false;
    }

    m_round = round;

    bool result = false;

    const auto reroll = m_priest->prayerRerolls();
    const auto mod = m_priest->prayerModifier();

    int prayingRoll = Dice::RollD6() + mod;
    if ((prayingRoll == 1) && (reroll == Rerolls::Ones)) {
        prayingRoll = Dice::RollD6() + mod;
    } else if ((prayingRoll < m_prayingValue) && (reroll == Rerolls::Failed)) {
        prayingRoll = Dice::RollD6() + mod;
    }
    if (prayingRoll >= m_prayingValue) {
        result = apply(prayingRoll, x, y);
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1, Wounds::Source::Prayer, this}, m_priest);
    }

    return result;
}

DamagePrayer::DamagePrayer(Unit *priest, const std::string &name, int prayingValue, int range, int damage,
                           int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_damage(damage) {
    m_allowedTargets = Abilities::Target::Enemy;
    m_effect = Abilities::EffectType::Damage;
}

int DamagePrayer::getDamage(Unit *target, int prayingRoll) const {
    return m_damage;
}

bool DamagePrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    int wounds = Dice::RollSpecial(getDamage(target, prayingRoll));
    target->applyDamage({0, wounds, Wounds::Source::Prayer, this}, m_priest);

    return true;
}

HealPrayer::HealPrayer(Unit *priest, const std::string &name, int prayingValue, int range, int healing, int damageOn1)
        :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_healing(healing) {
    m_allowedTargets = Abilities::Target::SelfAndFriendly;
    m_effect = Abilities::EffectType::Heal;
}

bool HealPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    int wounds = Dice::RollSpecial(getHealing(prayingRoll));
    target->heal(wounds);
    PLOG_INFO.printf("%s prays for %s with roll of %d (%d) heals %d wounds onto %s.",
                     m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, wounds,
                     target->name().c_str());

    return true;
}

BuffModifierPrayer::BuffModifierPrayer(Unit *priest, const std::string &name, int prayingValue, int range,
                                       Attribute which, int modifier, Abilities::Target allowedTargets,
                                       const std::vector<Keyword> &targetKeyword, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_attribute(which),
        m_modifier(modifier) {
    m_allowedTargets = allowedTargets;
    m_effect = (m_modifier > 0) ? Abilities::EffectType::Buff : Abilities::EffectType::Debuff;
}

bool BuffModifierPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    return target->buffModifier(m_attribute, m_modifier, {GamePhase::Hero, m_round + 1, m_priest->owningPlayer()});
}

int BuffModifierPrayer::getModifier(int prayingRoll) const {
    return m_modifier;
}

BuffRerollPrayer::BuffRerollPrayer(Unit *priest, const std::string &name, int prayingValue, int range,
                                   Attribute which, Rerolls reroll, Abilities::Target allowedTargets,
                                   const std::vector<Keyword> &targetKeyword, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_attribute(which),
        m_reroll(reroll) {
    m_allowedTargets = allowedTargets;
    m_effect = Abilities::EffectType::Buff;
}

bool BuffRerollPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    return target->buffReroll(m_attribute, m_reroll, {GamePhase::Hero, m_round + 1, m_priest->owningPlayer()});
}

BuffAbilityPrayer::BuffAbilityPrayer(Unit *priest, const std::string &name, int prayingValue, int range,
                                     Ability which, int value, Abilities::Target allowedTargets,
                                     const std::vector<Keyword> &targetKeyword, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_ability(which),
        m_value(value) {
    m_allowedTargets = allowedTargets;
    m_effect = (m_allowedTargets == Abilities::Target::Enemy) ? Abilities::EffectType::Debuff
                                                              : Abilities::EffectType::Buff;
}

int BuffAbilityPrayer::getValue(int prayingRoll) const {
    return m_value;
}

bool BuffAbilityPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr)
        return false;

    PLOG_INFO << m_priest->name() << " prays for " << name() << " with a roll of " << prayingRoll << " on to " << target->name();
    PLOG_INFO << "\tBuffing Ability: " << magic_enum::enum_name(m_ability) << ": " << m_value;

    target->buffAbility(m_ability, m_value, {GamePhase::Hero, m_round + 1, m_priest->owningPlayer()});

    return true;
}
