/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/InfernalEnrapturess.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 150;

    bool InfernalEnrapturess::s_registered = false;

    InfernalEnrapturess::InfernalEnrapturess(Host host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlaaneshBase(host, "Infernal Enrapturess Herald of Slaanesh", 6, g_wounds, 10, 5, false, g_pointsPerUnit),
            m_ravagingClaw(Weapon::Type::Melee, "Ravaging Claw", 1, 3, 3, 4, -1, 1),
            m_lyreCacophonousMelody(Weapon::Type::Missile, "Heartstring Lyre: Cacophonous Melody", 18, 6, 3, 4, -1, 1),
            m_lyreEuphonicBlast(Weapon::Type::Missile, "Heartstring Lyre: Euphonic Blast", 24, 1, 2, 3, -3, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, HERALD_OF_SLAANESH, INFERNAL_ENRAPTURESS};
        m_weapons = {&m_ravagingClaw, &m_lyreCacophonousMelody, &m_lyreEuphonicBlast};
        m_battleFieldRole = Role::Leader;

        // Discordant Disruption
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_lyreCacophonousMelody);
        model->addMissileWeapon(&m_lyreEuphonicBlast);
        model->addMeleeWeapon(&m_ravagingClaw);
        addModel(model);
    }

    Unit *InfernalEnrapturess::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new InfernalEnrapturess(host, trait, artefact, general);
    }

    void InfernalEnrapturess::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    InfernalEnrapturess::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    InfernalEnrapturess::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Infernal Enrapturess Herald of Slaanesh", factoryMethod);
        }
    }

    void InfernalEnrapturess::onStartShooting(PlayerId player) {
        if (player == owningPlayer()) {
            auto enemyRoster = Board::Instance()->getPlayerRoster(GetEnemyId(owningPlayer()));

            Unit *nearestUnit = enemyRoster ? enemyRoster->nearestUnit(this) : nullptr;
            if (nearestUnit) {
                double rangeTo = distanceTo(nearestUnit);
                if (rangeTo < (double) m_lyreCacophonousMelody.range()) {
                    m_lyreCacophonousMelody.activate(true);
                    m_lyreEuphonicBlast.activate(false);
                } else {
                    m_lyreCacophonousMelody.activate(false);
                    m_lyreEuphonicBlast.activate(true);
                }
            }
        }
    }

    int InfernalEnrapturess::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
