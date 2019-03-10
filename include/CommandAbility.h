/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COMMANDABILITY_H
#define COMMANDABILITY_H

#include <string>
#include "WarhammerSim.h"

class Unit;

class CommandAbility
{
public:
    CommandAbility(Unit* source, const std::string& name) :
        m_source(source),
        m_name(name) {}

    virtual int apply(const Unit* target) = 0;

protected:

    Unit* m_source;
    std::string m_name;
    Duration m_duration;
};

#endif// COMMANDABILITY_H