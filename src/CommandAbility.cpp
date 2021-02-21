/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <string>
#include "Unit.h"
#include "CommandAbility.h"

bool CommandAbility::use(Unit *target, int round) {
    m_round = round;
    return apply(target);
}

bool CommandAbility::use(double x, double y, int round) {
    m_round = round;
    return apply(x, y);
}

Duration CommandAbility::defaultDuration() const {
    return {m_phase, m_round, m_source->owningPlayer()};
}

int CommandAbility::range() const {
    return m_source->isGeneral() ? m_rangeGeneral : m_rangeHero;
}

BuffModifierCommandAbility::BuffModifierCommandAbility(Unit *general, const std::string &name, int rangeGeneral, int rangeHero, Phase phase,
                                                       BuffableAttribute which, int modifier, Abilities::Target allowedTargets,
                                                       const std::vector<Keyword>& targetKeyword) :
        CommandAbility(general, name, rangeGeneral, rangeHero, phase),
        m_attribute(which),
        m_modifier(modifier){
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = (m_modifier > 0) ? Abilities::EffectType::Buff : Abilities::EffectType::Debuff;
}

int BuffModifierCommandAbility::getModifier() const {
    return m_modifier;
}

bool BuffModifierCommandAbility::apply(Unit *target) {
    if (target == nullptr)
        return false;

    target->buffModifier(m_attribute, getModifier(), defaultDuration());

    return true;
}

BuffRerollCommandAbility::BuffRerollCommandAbility(Unit *general, const std::string &name, int rangeGeneral, int rangeHero, Phase phase,
                                                   BuffableAttribute which, Rerolls reroll,
                                                   Abilities::Target allowedTargets,
                                                   const std::vector<Keyword> &targetKeyword) :
        CommandAbility(general, name, rangeGeneral, rangeHero, phase),
        m_attribute(which),
        m_reroll(reroll) {
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = Abilities::EffectType::Buff;
}

bool BuffRerollCommandAbility::apply(Unit *target) {
    if (target == nullptr)
        return false;

    target->buffReroll(m_attribute, m_reroll, defaultDuration());

    return true;
}

BuffAbilityCommandAbility::BuffAbilityCommandAbility(Unit *general, const std::string &name, int rangeGeneral,
                                                     int rangeHero, Phase phase, BuffableAbility which, int value,
                                                     Abilities::Target allowedTargets,
                                                     const std::vector<Keyword> &targetKeyword) :
        CommandAbility(general, name, rangeGeneral, rangeHero, phase),
        m_ability(which),
        m_value(value) {
    m_allowedTargets = allowedTargets;
    m_targetKeywords = targetKeyword;
    m_effect = Abilities::EffectType::Buff;
}

bool BuffAbilityCommandAbility::apply(Unit *target) {
    if (target == nullptr)
        return false;

    target->buffAbility(m_ability, m_value, defaultDuration());

    return true;
}
