/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BladebringerHellflayer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 200;

    bool BladebringerOnHellflayer::s_registered = false;

    BladebringerOnHellflayer::BladebringerOnHellflayer() :
            SlaaneshBase("Bladebringer, Herald on Hellflayer", 12, g_wounds, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 6, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, HELLFLAYER,
                      BLADEBRINGER};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool BladebringerOnHellflayer::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateAcquiescence(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *BladebringerOnHellflayer::Create(const ParameterList &parameters) {
        auto unit = new BladebringerOnHellflayer();

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

    void BladebringerOnHellflayer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BladebringerOnHellflayer::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    BladebringerOnHellflayer::ComputePoints,
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
            s_registered = UnitFactory::Register("Bladebringer, Herald on Hellflayer", factoryMethod);
        }
    }

    int BladebringerOnHellflayer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int BladebringerOnHellflayer::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                               const Unit *target) const {
        auto extras = Unit::extraAttacks(attackingModel, weapon, target);

        // Soulscent
        if (!weapon->isMissile()) extras += m_extraMeleeAttacks;

        return extras;
    }

    void BladebringerOnHellflayer::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        m_extraMeleeAttacks = 0;

        // Soulscent
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
            for (auto u : units) {
                if (Dice::RollD6() >= 4) {
                    u->applyDamage({0, Dice::RollD3()}, this);

                    // Extra attacks
                    m_extraMeleeAttacks++;
                }
            }
        }
    }

} // Slannesh
