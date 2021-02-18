/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <string>
#include <vector>
#include "AgeOfSigmarSim.h"
#include "Abilities.h"

class Unit;

class CommandAbility {
public:
    CommandAbility(Unit *source, const std::string &name, int rangeGeneral, int rangeHero, Phase phase) :
            m_source(source),
            m_name(name),
            m_phase(phase),
            m_rangeGeneral(rangeGeneral),
            m_rangeHero(rangeHero) {}

    virtual ~CommandAbility() = default;

    virtual bool canBeUsed() const { return true; };
    virtual bool apply(Unit *target, int round) = 0;

    int range() const;

    const std::string &name() const { return m_name; }

    Abilities::Target allowedTargets() const { return m_allowedTargets; }
    const std::vector<Keyword>& allowedTargetKeywords() const { return m_targetKeywords; }

    Phase phase() const { return m_phase; }

protected:

    Duration defaultDuration() const;

    Unit *m_source = nullptr;
    std::string m_name;
    Duration m_duration;
    Phase m_phase = Phase::Hero;
    int m_round = 0;
    int m_rangeGeneral = 0;
    int m_rangeHero = 0;

    bool m_lineOfSiteRequired = true;
    Abilities::Target m_allowedTargets = Abilities::Target::None;
    Abilities::EffectType m_effect = Abilities::EffectType::Utility;
    std::vector<Keyword> m_targetKeywords;
};

class BuffModifierCommandAbility : public CommandAbility {
public:
    BuffModifierCommandAbility(Unit *general, const std::string &name, int rangeGeneral, int rangeHero, Phase phase,
                               BuffableAttribute which, int modifier, Abilities::Target allowedTargets,
                               const std::vector<Keyword>& targetKeywords = {});

    bool apply(Unit* target, int round) override;

protected:

    virtual int getModifier() const;

    BuffableAttribute m_attribute = To_Hit_Melee;
    int m_modifier = 0;
};

class BuffRerollCommandAbility : public CommandAbility {
public:
    BuffRerollCommandAbility(Unit *caster, const std::string &name, int rangeGeneral, int rangeHero, Phase phase, BuffableAttribute which,
                             Rerolls reroll, Abilities::Target allowedTargets, const std::vector<Keyword>& targetKeyword = {});

    bool apply(Unit* target, int round) override;

protected:

    BuffableAttribute m_attribute = To_Hit_Melee;
    Rerolls m_reroll = No_Rerolls;
};
