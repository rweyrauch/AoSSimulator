/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Sigvald.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 50;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 260;

    bool SigvaldPrinceOfSlaanesh::s_registered = false;

    SigvaldPrinceOfSlaanesh::SigvaldPrinceOfSlaanesh(Host host, bool isGeneral) :
            SlaaneshBase(host, "Sigvald Prince of Slaanesh", 6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_shardslash(Weapon::Type::Melee, "Shardslash", 1, 5, 2, 3, -2, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, SIGVALD};
        m_weapons = {&m_shardslash};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_shardslash);
        addModel(model);
    }

    Unit *SigvaldPrinceOfSlaanesh::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new SigvaldPrinceOfSlaanesh(host, general);
    }

    void SigvaldPrinceOfSlaanesh::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SigvaldPrinceOfSlaanesh::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    SigvaldPrinceOfSlaanesh::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("SigvaldPrince of Slaanesh", factoryMethod);
        }
    }

    int SigvaldPrinceOfSlaanesh::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void SigvaldPrinceOfSlaanesh::onCharged() {
        SlaaneshBase::onCharged();

        m_shardslash.setAttacks(std::min(5, m_unmodifiedChargeRoll));
    }

    void SigvaldPrinceOfSlaanesh::onBeginTurn(int battleRound, PlayerId player) {
        EventInterface::onBeginTurn(battleRound, player);
        m_shardslash.setAttacks(5);
    }

    Wounds SigvaldPrinceOfSlaanesh::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // The Mirror Shield
        return ignoreWounds(wounds, 4);
    }

} // Slannesh
