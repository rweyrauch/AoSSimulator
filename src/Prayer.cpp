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

DamagePrayer::DamagePrayer(Unit *priest, const std::string &name, int prayingValue, float range, int damage,
                           int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_damage(damage) {
    m_targetFriendly = false;
}

bool DamagePrayer::pray(Unit *target, int round) {
    if (target == nullptr) {
        return false;
    }

    // Distance to target
    const float distance = m_priest->distanceTo(target);
    if (distance > m_range) {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target)) {
        return false;
    }

    bool result = false;

    int mortalWounds = 0;
    const int prayingRoll = Dice::roll2D6();
    if (prayingRoll >= m_prayingValue) {
        mortalWounds = Dice::rollSpecial(getDamage(target, prayingRoll));
        target->applyDamage({0, mortalWounds});
        SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) inflicts %d mortal wounds into %s.\n",
               m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, mortalWounds,
               target->name().c_str());
        result = true;
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1});
    }

    return result;
}

int DamagePrayer::getDamage(Unit *target, int prayingRoll) const {
    return m_damage;
}

HealPrayer::HealPrayer(Unit *priest, const std::string &name, int prayingValue, float range, int healing, int damageOn1)
        :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_healing(healing) {
    m_targetFriendly = true;
}

bool HealPrayer::pray(Unit *target, int round) {
    if (target == nullptr) {
        return false;
    }

    // Distance to target
    const float distance = m_priest->distanceTo(target);
    if (distance > m_range) {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target)) {
        return false;
    }

    bool result = false;

    int wounds = 0;
    const int prayingRoll = Dice::roll2D6();
    if (prayingRoll >= m_prayingValue) {
        wounds = Dice::rollSpecial(getHealing(prayingRoll));
        target->heal(wounds);
        SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) heals %d wounds onto %s.\n",
               m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, wounds, target->name().c_str());
        result = true;
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1});
    }

    return result;
}

BuffModifierPrayer::BuffModifierPrayer(Unit *priest, const std::string &name, int prayingValue, float range,
                                       BuffableAttribute which, int modifier, bool targetFriendly, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_attribute(which),
        m_modifier(modifier) {
    m_targetFriendly = targetFriendly;
}

bool BuffModifierPrayer::pray(Unit *target, int round) {
    if (target == nullptr) {
        return false;
    }

    // Distance to target
    const float distance = m_priest->distanceTo(target);
    if (distance > m_range) {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target)) {
        return false;
    }

    bool result = false;

    const int prayingRoll = Dice::roll2D6();
    if (prayingRoll >= m_prayingValue) {
        target->buffModifier(m_attribute, m_modifier, {Phase::Hero, round + 1, m_priest->owningPlayer()});

        SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) onto %s.\n",
               m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, target->name().c_str());
        result = true;
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1});
    }

    return result;
}

int BuffModifierPrayer::getModifier(int prayingRoll) const {
    return m_modifier;
}

BuffRerollPrayer::BuffRerollPrayer(Unit *priest, const std::string &name, int prayingValue, float range,
                                   BuffableAttribute which, Rerolls reroll, bool targetFriendly, int damageOn1) :
        Prayer(priest, name, prayingValue, range, damageOn1),
        m_attribute(which),
        m_reroll(reroll) {
    m_targetFriendly = targetFriendly;
}

bool BuffRerollPrayer::pray(Unit *target, int round) {
    if (target == nullptr) {
        return false;
    }

    // Distance to target
    const float distance = m_priest->distanceTo(target);
    if (distance > m_range) {
        return false;
    }

    // Check for visibility to target
    if (!Board::Instance()->isVisible(m_priest, target)) {
        return false;
    }

    bool result = false;

    const int prayingRoll = Dice::roll2D6();
    if (prayingRoll >= m_prayingValue) {
        target->buffReroll(m_attribute, m_reroll, {Phase::Hero, round + 1, m_priest->owningPlayer()});

        SimLog(Verbosity::Narrative, "%s prays for %s with roll of %d (%d) onto %s.\n",
               m_priest->name().c_str(), name().c_str(), prayingRoll, m_prayingValue, target->name().c_str());
        result = true;
    } else if ((prayingRoll == 1) && (m_damageOn1 != 0)) {
        m_priest->applyDamage({0, m_damageOn1});
    }

    return result;
}
