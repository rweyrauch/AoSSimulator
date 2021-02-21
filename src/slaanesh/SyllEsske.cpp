/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SyllEsske.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 25;
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 200;

    bool SyllEsske::s_registered = false;

    SyllEsske::SyllEsske() :
            SlaaneshBase("Syll'Esske The Vengeful Allegiance", 8, g_wounds, 10, 4, false),
            m_axeOfDominion(Weapon::Type::Melee, "Axe of Dominion", 2, 4, 4, 3, -2, RAND_D3),
            m_scourgingWhip(Weapon::Type::Melee, "Scourging Whip", 2, 8, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, SYLL_ESSKE,
                      THE_VENGEFUL_ALLEGIANCE};
        m_weapons = {&m_axeOfDominion, &m_scourgingWhip};
        m_battleFieldRole = Leader;

        // Lithe and Swift
        m_runAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool SyllEsske::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axeOfDominion);
        model->addMeleeWeapon(&m_scourgingWhip);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffAbilitySpell>(this, "Subvert", 7, 18, Cannot_Use_Command_Abilities,
                     1, Abilities::Target::Enemy, std::vector<Keyword>{HERO}));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<BuffAbilityCommandAbility>(this, "Regal Authority", 18, 18, Phase::Battleshock, Ignore_Battleshock, 1,
                                                                                 Abilities::Target::Friendly, std::vector<Keyword>{SLAANESH}));
        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *SyllEsske::Create(const ParameterList &parameters) {
        auto unit = new SyllEsske();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

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

    void SyllEsske::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SyllEsske::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    SyllEsske::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Syll'Esske The Vengeful Allegiance", factoryMethod);
        }
    }

    int SyllEsske::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
