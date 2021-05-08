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
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 150;

    bool ShardspeakerOfSlaanesh::s_registered = false;

    ShardspeakerOfSlaanesh::ShardspeakerOfSlaanesh(Host host, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlaaneshBase(host, "Shardspeaker of Slaanesh", 6, g_wounds, 10, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Haze Staff", 1, 2, 4, 3, 0, RAND_D3),
            m_claws(Weapon::Type::Melee, "Shadow-cloaked Claws", 2, 4, 3, 3, -2, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, WIZARD, SHARDSPEAKER};
        m_weapons = {&m_staff, &m_claws};
        m_battleFieldRole = Role::Leader;

        m_claws.activate(false);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_staff);

        m_claws.activate(false);

        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffModifierSpell>(this, "Reflection Eternal", 6, 12.0, Attribute::To_Wound_Melee, -1,
                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *ShardspeakerOfSlaanesh::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ShardspeakerOfSlaanesh(host, lore, trait, artefact, general);
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

    int ShardspeakerOfSlaanesh::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ShardspeakerOfSlaanesh::onCastSpell(const Spell *spell, const Unit *target) {
        SlaaneshBase::onCastSpell(spell, target);

        // Turn on Mist Lurkers
        m_claws.activate(true);
    }

    int ShardspeakerOfSlaanesh::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = SlaaneshBase::toSaveModifier(weapon, attacker);

        // Mist Lurkers
        if (m_claws.isActive()) {
            mod += 2;
        }
        return mod;
    }

    void ShardspeakerOfSlaanesh::onStartHero(PlayerId player) {
        SlaaneshBase::onStartHero(player);

        // Turn off Mist Lurkers
        if (player == owningPlayer())
            m_claws.activate(false);
    }

} // Slannesh
