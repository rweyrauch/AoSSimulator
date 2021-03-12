/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "SummonAbility.h"
#include <Roster.h>
#include <UnitFactory.h>
#include <memory>

namespace FleshEaterCourt {

    SummonAbility::SummonAbility(Unit *leader, Roster* roster, const std::string &name, const SummonedUnitDesc& summonableUnits) :
            CommandAbility(leader, name, 0, 0, Phase::Movement),
            m_summonedUnitDesc(summonableUnits),
            m_roster(roster) {
        m_allowedTargets = Abilities::Target::Self;
        m_effect = Abilities::EffectType::Summon;
    }

    bool SummonAbility::apply(Unit* target) {

        // TODO: Select a unit at random if more than own available.
        std::string summonedUnitName = m_summonedUnitDesc.front().first;

        // Summon a unit and add to roster.
        auto factory = UnitFactory::LookupUnit(summonedUnitName);
        if (factory) {
            if (m_roster) {
                // TODO: Set the number of models to add to the unit
                auto parameters = factory->m_parameters;
                auto unit = std::shared_ptr<Unit>(UnitFactory::Create(summonedUnitName, parameters));
                unit->deploy(m_source->position(), m_source->orientation());
                m_roster->addUnit(unit);
            }
        }
        return true;
    }
}