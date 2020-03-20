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
#include "AgeOfSigmarSim.h"

class Unit;

class CommandAbility {
public:
    CommandAbility(Unit *source, const std::string &name) :
            m_source(source),
            m_name(name) {}

    virtual ~CommandAbility() = default;

    virtual int apply(const Unit *target) = 0;

    float range() const { return m_range; }

    const std::string &name() const { return m_name; }

    bool targetFriendly() const { return m_targetFriendly; }

    Phase phase() const { return m_phase; }

protected:

    Unit *m_source = nullptr;
    std::string m_name = "";
    //Duration m_duration;
    float m_range = 0.0f;

    bool m_targetFriendly = true;
    Phase m_phase = Phase::Hero;
};

#endif// COMMANDABILITY_H