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
#include "AoSKeywords.h"
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

    bool use(Unit *target, int round);

    bool use(double x, double y, int round);

    int range() const;

    const std::string &name() const { return m_name; }

    Abilities::Target allowedTargets() const { return m_allowedTargets; }
    const std::vector<Keyword>& allowedTargetKeywords() const { return m_targetKeywords; }

    Phase phase() const { return m_phase; }

protected:

    virtual bool apply(Unit *target) = 0;
    virtual bool apply(double x, double y) = 0;

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
    BuffModifierCommandAbility(Unit *source, const std::string &name, int rangeGeneral, int rangeHero, Phase phase,
                               Attribute which, int modifier, Abilities::Target allowedTargets,
                               const std::vector<Keyword>& targetKeywords = {});


protected:

    bool apply(Unit* target) override;
    bool apply(double x, double y) override { return false; }

    virtual int getModifier() const;

    Attribute m_attribute = Attribute::To_Hit_Melee;
    int m_modifier = 0;
};

class BuffRerollCommandAbility : public CommandAbility {
public:
    BuffRerollCommandAbility(Unit *source, const std::string &name, int rangeGeneral, int rangeHero, Phase phase, Attribute which,
                             Rerolls reroll, Abilities::Target allowedTargets, const std::vector<Keyword>& targetKeyword = {});

protected:

    bool apply(Unit* target) override;
    bool apply(double x, double y) override { return false; }

    Attribute m_attribute = Attribute::To_Hit_Melee;
    Rerolls m_reroll = Rerolls::None;
};

class BuffAbilityCommandAbility : public CommandAbility {
public:
    BuffAbilityCommandAbility(Unit *source, const std::string &name, int rangeGeneral, int rangeHero, Phase phase, Ability which,
                              int value, Abilities::Target allowedTargets, const std::vector<Keyword>& targetKeyword = {});

protected:

    bool apply(Unit* target) override;
    bool apply(double x, double y) override { return false; }

    Ability m_ability = Ability::Ignore_Battleshock;
    int m_value = 0;
};
