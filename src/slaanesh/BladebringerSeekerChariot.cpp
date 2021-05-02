/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BladebringerSeekerChariot.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 190;

    bool BladebringerOnSeekerChariot::s_registered = false;

    BladebringerOnSeekerChariot::BladebringerOnSeekerChariot() :
            SlaaneshBase("Bladebringer, Herald on Seeker Chariot", 12, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, SEEKER_CHARIOT,
                      BLADEBRINGER};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_piercingClaws.setMount(true);
        m_poisonedTongues.setMount(true);

        // Impossibly Swift
        m_retreatAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void BladebringerOnSeekerChariot::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateAcquiescence(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *BladebringerOnSeekerChariot::Create(const ParameterList &parameters) {
        auto unit = new BladebringerOnSeekerChariot();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        unit->configure(lore);
        return unit;
    }

    void BladebringerOnSeekerChariot::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BladebringerOnSeekerChariot::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    BladebringerOnSeekerChariot::ComputePoints,
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
            s_registered = UnitFactory::Register("Bladebringer, Herald on Seeker Chariot", factoryMethod);
        }
    }

    void BladebringerOnSeekerChariot::onCharged() {
        SlaaneshBase::onCharged();

        // Multilating Blades
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            if (Dice::RollD6() >= 2) {
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    int BladebringerOnSeekerChariot::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
