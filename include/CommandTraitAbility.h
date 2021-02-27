/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <string>
#include <vector>
#include "AgeOfSigmarSim.h"
#include "Abilities.h"

class Unit;

class CommandTraitAbility {
public:
    CommandTraitAbility(Unit *source, const std::string &name) :
            m_source(source),
            m_name(name) {}

    virtual ~CommandTraitAbility() = default;

    virtual int apply(const Unit *target) = 0;

    double range() const { return m_range; }

    const std::string &name() const { return m_name; }

    Abilities::Target allowedTargets() const { return m_allowedTargets; }
    const std::vector<Keyword>& allowedTargetKeywords() const { return m_targetKeywords; }

    Phase phase() const { return m_phase; }

protected:

    Unit *m_source = nullptr;
    std::string m_name;
    Duration m_duration;
    Phase m_phase = Phase::Hero;
    int m_range = 0;

    bool m_lineOfSiteRequired = true;
    Abilities::Target m_allowedTargets = Abilities::Target::None;
    Abilities::EffectType m_effect = Abilities::EffectType::Utility;
    std::vector<Keyword> m_targetKeywords;
};
