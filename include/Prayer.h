/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PRAYER_H
#define PRAYER_H

#include <string>
#include "WarhammerSim.h"

class Unit;

class Prayer
{
public:
    explicit Prayer(Unit* priest, const std::string& name, int prayingValue, float range, int damageOn1 = 0) :
        m_priest(priest),
        m_name(name),
        m_prayingValue(prayingValue),
        m_range(range),
        m_damageOn1(damageOn1) {}

    virtual bool pray(Unit* target, int round) = 0;
    virtual bool pray(float x, float y, int round) = 0;

    int prayingValue() const { return m_prayingValue; }
    float range() const { return m_range; }
    const std::string& name() const { return m_name; }

    bool targetFriendly() const { return m_targetFriendly; }

protected:

    Unit* m_priest = nullptr;
    std::string m_name;
    int m_prayingValue = 0;
    Duration m_duration{};
    float m_range = 0.0f;
    int m_damageOn1 = 0;

    bool m_targetFriendly = false;
};


class DamagePrayer : public Prayer
{
public:
    DamagePrayer(Unit* priest, const std::string& name, int prayingValue, float range, int damage, int damageOn1 = 0);

    bool pray(Unit* target, int round) override;
    bool pray(float x, float y, int round) override { return false; }

protected:

    virtual int getDamage(int prayingRoll) const;

    int m_damage = 0;
};

class HealPrayer : public Prayer
{
public:
    HealPrayer(Unit* priest, const std::string& name, int prayingValue, float range, int healing, int damageOn1 = 0);

    bool pray(Unit* target, int round) override;
    bool pray(float x, float y, int round) override { return false; }

protected:

    virtual int getHealing(int prayingRoll) const
    {
        return m_healing;
    }

    int m_healing = 0;
};

class BuffModifierPrayer : public Prayer
{
public:
    BuffModifierPrayer(Unit* priest, const std::string& name, int prayingValue, float range,
        BuffableAttribute which, int modifier, bool targetFriendly, int damageOn1 = 0);

    bool pray(Unit* target, int round) override;
    bool pray(float x, float y, int round) override { return false; }

protected:

    virtual int getModifier(int prayingRoll) const;

    BuffableAttribute m_attribute = ToHit;
    int m_modifier = 0;
};

class BuffRerollPrayer : public Prayer
{
public:
    BuffRerollPrayer(Unit* priest, const std::string& name, int prayingValue, float range,
        BuffableAttribute which, Rerolls reroll, bool targetFriendly, int damageOn1 = 0);

    bool pray(Unit* target, int round) override;
    bool pray(float x, float y, int round) override { return false; }

protected:

    BuffableAttribute m_attribute = ToHit;
    Rerolls m_reroll = NoRerolls;
};

#endif// PRAYER_H