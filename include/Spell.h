/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPELL_H
#define SPELL_H

#include <string>
#include "WarhammerSim.h"

class Unit;

class Spell
{
public:
    Spell(Unit* caster, const std::string& name, int castingValue, float range) :
        m_caster(caster),
        m_name(name),
        m_castingValue(castingValue),
        m_range(range) {}


    int castingValue() const { return m_castingValue; }
    float range() const { return m_range; }
    const std::string& name() const { return m_name; }

    bool targetFriendly() const { return m_targetFriendly; }

    virtual bool cast(Unit* target, int round) = 0;
    virtual bool cast(float x, float y, int round) = 0;

protected:

    Unit* m_caster = nullptr;
    std::string m_name;
    int m_castingValue = 0;
    float m_range = 0.0f;

    bool m_targetFriendly = false;
};

class DamageSpell : public Spell
{
public:
    DamageSpell(Unit* caster, const std::string& name, int castingValue, float range, int damage);

    bool cast(Unit* target, int round) override;
    bool cast(float x, float y, int round) override { return false; }

protected:

    virtual int getDamage(int castingRoll) const { return m_damage; }
    int m_damage = 0;
};

#endif// SPELL_H