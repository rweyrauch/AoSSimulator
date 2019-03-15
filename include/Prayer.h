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
    explicit Prayer(int castingValue) :
            m_castingValue(castingValue) {}

    virtual int pray(const Unit* target) = 0;
    virtual int pray(float x, float y) = 0;

protected:

    std::string m_name;
    int m_castingValue = 0;
    Duration m_duration{};
};

#endif// PRAYER_H