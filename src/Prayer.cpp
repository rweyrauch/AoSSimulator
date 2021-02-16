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

    const int prayingRoll = Dice::RollD6();
    if (prayingRoll >= m_prayingValue) {
        result = apply(prayingRoll, target);
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1}, m_priest);
    }

    return result;
}

bool Prayer::pray(double x, double y, int round) {
    const Math::Point3 targetPoint(x, y, 0.0);

    // Distance to point
    const double distance = m_priest->position().distance(targetPoint);
    if (distance > (double)Dice::RollSpecial(m_range)) {
        return false;
    }

    m_round = round;

    bool result = false;

    const int prayingRoll = Dice::RollD6();
    if (prayingRoll >= m_prayingValue) {
        result = apply(prayingRoll, x, y);
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1}, m_priest);
    }

    return result;
}

DamagePrayer::DamagePrayer(Unit *priest, const std::string &name, int prayingValue, double range, int damage,
                           int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_damage(damage) {
    m_allowedTargets = Prayer::Target::Enemy;
    m_effect = EffectType::Damage;
}

int DamagePrayer::getDamage(Unit *target, int prayingRoll) const {
    return m_damage;
}

bool DamagePrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    int wounds = Dice::RollSpecial(getDamage(target, prayingRoll));
    target->applyDamage({0, wounds}, m_priest);

    return true;
}

HealPrayer::HealPrayer(Unit *priest, const std::string &name, int prayingValue, double range, int healing, int damageOn1)
        :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_healing(healing) {
    m_allowedTargets = Prayer::Target::SelfAndFriendly;
    m_effect = EffectType::Heal;
}

bool HealPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    int wounds = Dice::RollSpecial(getHealing(prayingRoll));
    target->heal(wounds);
    SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) heals %d wounds onto %s.\n",
           m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, wounds, target->name().c_str());

    return true;
}

BuffModifierPrayer::BuffModifierPrayer(Unit *priest, const std::string &name, int prayingValue, double range,
                                       BuffableAttribute which, int modifier, Target allowedTargets, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_attribute(which),
        m_modifier(modifier) {
    m_allowedTargets = allowedTargets;
    m_effect = (m_modifier > 0) ? EffectType::Buff : EffectType::Debuff;
}

bool BuffModifierPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    return target->buffModifier(m_attribute, m_modifier, {Phase::Hero, m_round + 1, m_priest->owningPlayer()});
}

int BuffModifierPrayer::getModifier(int prayingRoll) const {
    return m_modifier;
}

BuffRerollPrayer::BuffRerollPrayer(Unit *priest, const std::string &name, int prayingValue, double range,
                                   BuffableAttribute which, Rerolls reroll, Target allowedTargets, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_attribute(which),
        m_reroll(reroll) {
    m_allowedTargets = allowedTargets;
    m_effect = EffectType::Buff;
}

bool BuffRerollPrayer::apply(int prayingRoll, Unit *target) {
    if (target == nullptr) {
        return false;
    }

    return target->buffReroll(m_attribute, m_reroll, {Phase::Hero, m_round + 1, m_priest->owningPlayer()});
}
