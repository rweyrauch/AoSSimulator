/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <string>
#include <utility>
#include "AgeOfSigmarSim.h"

class Unit;

class Spell {
public:

    enum class Result {
        Failed,
        Unbound,
        Success,
    };

    enum class Target {
        None,
        Point,
        Friendly,
        Enemy,
        Self,
        SelfAndFriendly
    };

    enum class EffectType {
        Utility,
        Damage,
        AreaOfEffectDamage,
        Heal,
        Buff,
        Debuff
    };

    Spell(Unit *caster, std::string name, int castingValue, int range) :
            m_caster(caster),
            m_name(std::move(name)),
            m_castingValue(castingValue),
            m_range(range) {}

    virtual ~Spell() = default;

    int castingValue() const { return m_castingValue; }

    int range() const { return m_range; }

    const std::string &name() const { return m_name; }

    Target allowedTargets() const { return m_allowedTargets; }
    const std::vector<Keyword>& allowedTargetKeywords() const { return m_targetKeywords; }

    Result cast(Unit *target, int round);

    Result cast(double x, double y, int round);

protected:

    virtual Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) = 0;
    virtual Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) = 0;

    Duration defaultDuration() const;

    Unit *m_caster = nullptr;
    std::string m_name;
    int m_castingValue = 0;
    int m_round = 0;
    int m_range = 0;

    bool m_lineOfSiteRequired = true;
    Target m_allowedTargets = Target::None;
    EffectType m_effect = EffectType::Utility;
    std::vector<Keyword> m_targetKeywords;
};

class DamageSpell : public Spell {
public:
    DamageSpell(Unit *caster, const std::string &name, int castingValue, int range, int damage,
                int castingValue2 = -1, int damage2 = -1);

protected:

    Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
    Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }

    virtual int getDamage(int castingRoll) const;

    int m_damage = 0;
    int m_castingValue2 = -1;
    int m_damage2 = -1;
};

DamageSpell *CreateArcaneBolt(Unit *caster);

class AreaOfEffectSpell : public Spell {
public:
    AreaOfEffectSpell(Unit *caster, const std::string &name, int castingValue, int range, int radius, int damage,
                      int affectedRoll);

protected:

    Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
    Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override;

    virtual int getDamage(int castingRoll) const;
    virtual void secondaryEffect(Unit *target, int round) const {}

    int m_damage = 0;
    int m_radius = 0;
    int m_affectedRoll = 0;
};

class LineOfEffectSpell : public Spell {
public:
    LineOfEffectSpell(Unit *caster, const std::string &name, int castingValue, int range, int damage,
                      int affectedRoll);

protected:

    Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
    Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override;

    virtual int getDamage(int castingRoll) const;

    int m_damage = 0;
    int m_affectedRoll = 0;

};

class HealSpell : public Spell {
public:
    HealSpell(Unit *caster, const std::string &name, int castingValue, int range, int healing, int castingValue2 = -1,
              int healing2 = -1, const std::vector<Keyword>& targetKeywords = {});

protected:

    Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
    Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }

    virtual int getHealing(int castingRoll) const;

    int m_healing = 0;
    int m_castingValue2 = -1;
    int m_healing2 = -1;
};

class BuffModifierSpell : public Spell {
public:
    BuffModifierSpell(Unit *caster, const std::string &name, int castingValue, int range,
                      BuffableAttribute which, int modifier, Target allowedTargets, const std::vector<Keyword>& targetKeywords = {});

protected:

    Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
    Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }

    virtual int getModifier(int castingRoll) const;

    BuffableAttribute m_attribute = To_Hit_Melee;
    int m_modifier = 0;
};

class BuffRerollSpell : public Spell {
public:
    BuffRerollSpell(Unit *caster, const std::string &name, int castingValue, int range, BuffableAttribute which,
                    Rerolls reroll, Target allowedTargets, const std::vector<Keyword>& targetKeyword = {});

protected:

    Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
    Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }

    BuffableAttribute m_attribute = To_Hit_Melee;
    Rerolls m_reroll = No_Rerolls;
};
