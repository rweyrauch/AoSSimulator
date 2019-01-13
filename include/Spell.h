/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPELL_H
#define SPELL_H

#include "WarhammerSim.h"

class Spell
{
public:
    Spell(int castingValue) :
        m_castingValue(castingValue) {}

    virtual int cast(const Unit* target) = 0;
    virtual int cast(float x, float y) = 0;

protected:
    std::string m_name;
    int m_castingValue = 0;
    Duration m_duration = CurrentPhase;
};

#endif// SPELL_H