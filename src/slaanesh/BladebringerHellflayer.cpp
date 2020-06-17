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
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 180;

    bool BladebringerOnHellflayer::s_registered = false;

    BladebringerOnHellflayer::BladebringerOnHellflayer() :
            SlaaneshBase("Bladebringer, Herald on Hellflayer", 12, WOUNDS, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 6, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, HELLFLAYER,
                      BLADEBRINGER};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool BladebringerOnHellflayer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

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
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore)
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Bladebringer, Herald on Hellflayer", factoryMethod);
        }
    }

    int BladebringerOnHellflayer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
