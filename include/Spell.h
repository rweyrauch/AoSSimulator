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
    Spell(Unit* caster, const std::string& name, int castingValue) :
        m_caster(caster),
        m_name(name),
        m_castingValue(castingValue) {}

    virtual int cast(const Unit* target) = 0;
    virtual int cast(float x, float y) = 0;

protected:

    Unit* m_caster;
    std::string m_name;
    int m_castingValue = 0;
    Duration m_duration = CurrentPhase;
};

class DamageSpell : public Spell
{
public:
    DamageSpell(Unit* caster, const std::string& name, int castingValue, int range, int damage);

    int cast(const Unit* target) override;
    int cast(float x, float y) override { return 0; }

protected:
    int m_range = 0;
    int m_damage = 0;
};

class ArcaneBolt : public DamageSpell
{
public:
    ArcaneBolt(Unit* caster);

};

#endif// SPELL_H