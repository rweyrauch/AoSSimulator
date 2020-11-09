/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Spell.h>
#include <Unit.h>
#include <Board.h>
#include <MathUtils.h>

DamageSpell::DamageSpell(Unit *caster, const std::string &name, int castingValue, double range, int damage,
                         int castingValue2, int damage2) :
        Spell(caster, name, castingValue, range),
        m_damage(damage),
        m_castingValue2(castingValue2),
        m_damage2(damage2) {
    m_targetFriendly = false;
}

Spell::Result DamageSpell::cast(Unit *target, int /*round*/) {
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

    int mortalWounds = 0;
    const int castingRoll = m_caster->rollCasting();
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            mortalWounds = Dice::RollSpecial(getDamage(castingRoll));
            target->applyDamage({0, mortalWounds, Wounds::Source::Spell});
            SimLog(Verbosity::Narrative,
                   "%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
                   m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, mortalWounds,
                   target->name().c_str());
            result = Result::Success;
        } else {
            result = Result::Unbound;
        }
    }

    return result;
}

int DamageSpell::getDamage(int castingRoll) const {
    if ((m_castingValue2 > 0) && (castingRoll >= m_castingValue2)) {
        return m_damage2;
    }
    return m_damage;
}

DamageSpell *CreateArcaneBolt(Unit *caster) {
    return new DamageSpell(caster, "Arcane Bolt", 5, 18.0, 1, 10, RAND_D3);
}

AreaOfEffectSpell::AreaOfEffectSpell(Unit *caster, const std::string &name, int castingValue, double range, double radius,
                                     int damage, int affectedRoll) :
        Spell(caster, name, castingValue, range),
        m_damage(damage),
        m_radius(radius),
        m_affectedRoll(affectedRoll) {
    m_targetFriendly = false;
}

Spell::Result AreaOfEffectSpell::cast(double x, double y, int round) {
    const Math::Point3 targetPoint(x, y, 0.0);

    // Distance to point
    const double distance = m_caster->position().distance(targetPoint);
    if (distance > m_range) {
        return Result::Failed;
    }

    Spell::Result result = Result::Failed;

    int mortalWounds = 0;
    const int castingRoll = m_caster->rollCasting();
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            auto units = Board::Instance()->getUnitsWithin(targetPoint, GetEnemyId(m_caster->owningPlayer()), m_radius);
            for (auto target : units) {
                bool unitAffected = true;
                if (m_affectedRoll != 0) {
                    int roll = Dice::RollD6();
                    unitAffected = (roll >= m_affectedRoll);
                }

                if (unitAffected) {
                    mortalWounds = Dice::RollSpecial(getDamage(castingRoll));
                    target->applyDamage({0, mortalWounds, Wounds::Source::Spell});
                    secondaryEffect(target, round);
                    SimLog(Verbosity::Narrative,
                           "%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
                           m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, mortalWounds,
                           target->name().c_str());
                }
            }
            result = Result::Success;
        } else {
            result = Result::Unbound;
        }
    }

    return result;
}

int AreaOfEffectSpell::getDamage(int /*castingRoll*/) const {
    return m_damage;
}

LineOfEffectSpell::LineOfEffectSpell(Unit *caster, const std::string &name, int castingValue, double range, int damage,
                                     int affectedRoll) :
        Spell(caster, name, castingValue, range),
        m_damage(damage),
        m_affectedRoll(affectedRoll) {
    m_targetFriendly = false;
}

Spell::Result LineOfEffectSpell::cast(double x, double y, int round) {
    return Result::Failed;
}

int LineOfEffectSpell::getDamage(int castingRoll) const {
    return m_damage;
}

HealSpell::HealSpell(Unit *caster, const std::string &name, int castingValue, double range, int healing,
                     int castingValue2, int healing2) :
        Spell(caster, name, castingValue, range),
        m_healing(healing),
        m_castingValue2(castingValue2),
        m_healing2(healing2) {
    m_targetFriendly = true;
}

Spell::Result HealSpell::cast(Unit *target, int round) {
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

    int wounds = 0;
    const int castingRoll = m_caster->rollCasting();
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            wounds = Dice::RollSpecial(getHealing(castingRoll));
            target->heal(wounds);
            SimLog(Verbosity::Narrative, "%s spell %s with casting roll of %d (%d) heals %d wounds onto %s.\n",
                   m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, wounds,
                   target->name().c_str());
            result = Result::Success;
        } else {
            result = Result::Unbound;
        }
    }

    return result;
}

int HealSpell::getHealing(int castingRoll) const {
    if ((m_castingValue2 > 0) && (castingRoll >= m_castingValue2)) {
        return m_healing2;
    }
    return m_healing;
}

BuffModifierSpell::BuffModifierSpell(Unit *caster, const std::string &name, int castingValue, double range,
                                     BuffableAttribute which, int modifier, bool targetFriendly) :
        Spell(caster, name, castingValue, range),
        m_attribute(which),
        m_modifier(modifier) {
    m_targetFriendly = targetFriendly;
}

Spell::Result BuffModifierSpell::cast(Unit *target, int round) {
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
            target->buffModifier(m_attribute, m_modifier, {Phase::Hero, round + 1, m_caster->owningPlayer()});
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

int BuffModifierSpell::getModifier(int /*castingRoll*/) const {
    return m_modifier;
}

BuffRerollSpell::BuffRerollSpell(Unit *caster, const std::string &name, int castingValue, double range,
                                 BuffableAttribute which, Rerolls reroll, bool targetFriendly) :
        Spell(caster, name, castingValue, range),
        m_attribute(which),
        m_reroll(reroll) {
    m_targetFriendly = targetFriendly;
}

Spell::Result BuffRerollSpell::cast(Unit *target, int round) {
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
            target->buffReroll(m_attribute, m_reroll, {Phase::Hero, round + 1, m_caster->owningPlayer()});
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
