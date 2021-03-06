/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Viceleader.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool ViceleaderHeraldOfSlaanesh::s_registered = false;

    ViceleaderHeraldOfSlaanesh::ViceleaderHeraldOfSlaanesh(Host host, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlaaneshBase(host, "Viceleader Herald of Slaanesh", 6, g_wounds, 10, 5, false, g_pointsPerUnit),
            m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 6, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, VICELEADER};
        m_weapons = {&m_ravagingClaws};
        m_battleFieldRole = Role::Leader;

        // Lithe and Swift
        m_runAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_ravagingClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateAcquiescence(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *ViceleaderHeraldOfSlaanesh::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ViceleaderHeraldOfSlaanesh(host, lore, trait, artefact, general);
    }

    void ViceleaderHeraldOfSlaanesh::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ViceleaderHeraldOfSlaanesh::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ViceleaderHeraldOfSlaanesh::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Viceleader Herald of Slaanesh", factoryMethod);
        }
    }

    Wounds ViceleaderHeraldOfSlaanesh::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Lightning Reflexes
        int numSixes = 0;
        Wounds totalWounds = ignoreWounds(wounds, 5, numSixes);
        return SlaaneshBase::applyWoundSave(totalWounds, attackingUnit);
    }

    int ViceleaderHeraldOfSlaanesh::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
