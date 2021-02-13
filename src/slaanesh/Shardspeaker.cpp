/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Shardspeaker.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 0;

    bool ShardspeakerOfSlaanesh::s_registered = false;

    ShardspeakerOfSlaanesh::ShardspeakerOfSlaanesh() :
            SlaaneshBase("Shardspeaker of Slaanesh", 6, g_wounds, 10, 5, false),
            m_staff(Weapon::Type::Melee, "Haze Staff", 1, 2, 4, 3, 0, RAND_D3),
            m_claws(Weapon::Type::Melee, "Shadow-cloaked Claws", 2, 4, 3, 3, -2, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, WIZARD, SHARDSPEAKER};
        m_weapons = {&m_staff, &m_claws};
        m_battleFieldRole = Leader;

        m_claws.activate(false);

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ShardspeakerOfSlaanesh::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *ShardspeakerOfSlaanesh::Create(const ParameterList &parameters) {
        auto unit = new ShardspeakerOfSlaanesh();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ShardspeakerOfSlaanesh::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ShardspeakerOfSlaanesh::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ShardspeakerOfSlaanesh::ComputePoints,
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
            s_registered = UnitFactory::Register("Shardspeaker of Slaanesh", factoryMethod);
        }
    }

    int ShardspeakerOfSlaanesh::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
