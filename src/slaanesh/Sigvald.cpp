/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Sigvald.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 50;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool SigvaldPrinceOfSlaanesh::s_registered = false;

    SigvaldPrinceOfSlaanesh::SigvaldPrinceOfSlaanesh() :
            SlaaneshBase("Sigvald Prince of Slaanesh", 6, g_wounds, 7, 4, false),
            m_shardslash(Weapon::Type::Melee, "Shardslash", 1, 5, 2, 3, -2, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, SIGVALD};
        m_weapons = {&m_shardslash};
        m_battleFieldRole = Leader;
    }

    bool SigvaldPrinceOfSlaanesh::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_shardslash);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *SigvaldPrinceOfSlaanesh::Create(const ParameterList &parameters) {
        auto unit = new SigvaldPrinceOfSlaanesh();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    int SigvaldPrinceOfSlaanesh::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void SigvaldPrinceOfSlaanesh::onCharged() {
        EventInterface::onCharged();

        m_shardslash.setAttacks(std::min(5, m_unmodifiedChargeRoll));
    }

    void SigvaldPrinceOfSlaanesh::onBeginTurn(int battleRound) {
        EventInterface::onBeginTurn(battleRound);
        m_shardslash.setAttacks(5);
    }

    Wounds SigvaldPrinceOfSlaanesh::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // The Mirror Shield
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(4);
        totalWounds.mortal -= mortalSaves.rollsGE(4);

        return totalWounds;
     }

} // Slannesh
