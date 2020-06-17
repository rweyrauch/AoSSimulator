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
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool ViceleaderHeraldOfSlaanesh::s_registered = false;

    ViceleaderHeraldOfSlaanesh::ViceleaderHeraldOfSlaanesh() :
            SlaaneshBase("Viceleader Herald of Slaanesh", 6, WOUNDS, 10, 5, false),
            m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 6, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, VICELEADER};
        m_weapons = {&m_ravagingClaws};

        // Lithe and Swift
        m_runAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ViceleaderHeraldOfSlaanesh::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_ravagingClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *ViceleaderHeraldOfSlaanesh::Create(const ParameterList &parameters) {
        auto unit = new ViceleaderHeraldOfSlaanesh();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore)
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Viceleader Herald of Slaanesh", factoryMethod);
        }
    }

    Wounds ViceleaderHeraldOfSlaanesh::applyWoundSave(const Wounds &wounds) {
        // Lightning Reflexes
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int ViceleaderHeraldOfSlaanesh::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
