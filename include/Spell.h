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

    Spell(Unit *caster, std::string name, int castingValue, double range) :
            m_caster(caster),
            m_name(std::move(name)),
            m_castingValue(castingValue),
            m_range(range) {}

    virtual ~Spell() = default;

    int castingValue() const { return m_castingValue; }

    double range() const { return m_range; }

    const std::string &name() const { return m_name; }

    bool targetFriendly() const { return m_targetFriendly; }

    virtual Result cast(Unit *target, int round) = 0;

    virtual Result cast(double x, double y, int round) = 0;

protected:

    Unit *m_caster = nullptr;
    std::string m_name;
    int m_castingValue = 0;
    double m_range = 0.0f;

    bool m_targetFriendly = false;
};

class DamageSpell : public Spell {
public:
    DamageSpell(Unit *caster, const std::string &name, int castingValue, double range, int damage,
                int castingValue2 = -1, int damage2 = -1);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return Result::Failed; }

protected:

    virtual int getDamage(int castingRoll) const;

    int m_damage = 0;
    int m_castingValue2 = -1;
    int m_damage2 = -1;
};

DamageSpell *CreateArcaneBolt(Unit *caster);

class AreaOfEffectSpell : public Spell {
public:
    AreaOfEffectSpell(Unit *caster, const std::string &name, int castingValue, double range, double radius, int damage,
                      int affectedRoll);

    Result cast(Unit *target, int round) override { return Result::Failed; }

    Result cast(double x, double y, int round) override;

protected:

    [[nodiscard]] virtual int getDamage(int castingRoll) const;

    virtual void secondaryEffect(Unit *target, int round) const {}

    int m_damage = 0;
    double m_radius = 0.0f;
    int m_affectedRoll = 0;
};

class LineOfEffectSpell : public Spell {
public:
    LineOfEffectSpell(Unit *caster, const std::string &name, int castingValue, double range, int damage,
                      int affectedRoll);

    Result cast(Unit *target, int round) override { return Result::Failed; }

    Result cast(double x, double y, int round) override;

protected:

    [[nodiscard]] virtual int getDamage(int castingRoll) const;

    int m_damage = 0;
    int m_affectedRoll = 0;

};

class HealSpell : public Spell {
public:
    HealSpell(Unit *caster, const std::string &name, int castingValue, double range, int healing, int castingValue2 = -1,
              int healing2 = -1);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return Result::Failed; }

protected:

    [[nodiscard]] virtual int getHealing(int castingRoll) const;

    int m_healing = 0;
    int m_castingValue2 = -1;
    int m_healing2 = -1;
};

class BuffModifierSpell : public Spell {
public:
    BuffModifierSpell(Unit *caster, const std::string &name, int castingValue, double range,
                      BuffableAttribute which, int modifier, bool targetFriendly);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return Result::Failed; }

protected:

    [[nodiscard]] virtual int getModifier(int castingRoll) const;

    BuffableAttribute m_attribute = To_Hit_Melee;
    int m_modifier = 0;
};

class BuffRerollSpell : public Spell {
public:
    BuffRerollSpell(Unit *caster, const std::string &name, int castingValue, double range, BuffableAttribute which,
                    Rerolls reroll, bool targetFriendly);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return Result::Failed; }

protected:

    BuffableAttribute m_attribute = To_Hit_Melee;
    Rerolls m_reroll = No_Rerolls;
};
