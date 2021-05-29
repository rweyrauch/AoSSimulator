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
    static const int g_basesize = 50;
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 200;

    bool SyllEsske::s_registered = false;

    SyllEsske::SyllEsske(Host host, Lore lore, bool isGeneral) :
            SlaaneshBase(host, "Syll'Esske The Vengeful Allegiance", 8, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_axeOfDominion(Weapon::Type::Melee, "Axe of Dominion", 2, 4, 4, 3, -2, RAND_D3),
            m_scourgingWhip(Weapon::Type::Melee, "Scourging Whip", 2, 8, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, SYLL_ESSKE,
                      THE_VENGEFUL_ALLEGIANCE};
        m_weapons = {&m_axeOfDominion, &m_scourgingWhip};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_axeOfDominion.setMount(true);
        // Lithe and Swift
        m_runAndCharge = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axeOfDominion);
        model->addMeleeWeapon(&m_scourgingWhip);
        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffAbilitySpell>(this, "Subvert", 7, 18, Ability::Cannot_Use_Command_Abilities,
                                                   1, Abilities::Target::Enemy, std::vector<Keyword>{HERO}));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(
                std::make_unique<BuffAbilityCommandAbility>(this, "Regal Authority", 18, 18, GamePhase::Battleshock,
                                                            Ability::Ignore_Battleshock, 1,
                                                            Abilities::Target::Friendly,
                                                            std::vector<Keyword>{SLAANESH}));
    }

    Unit *SyllEsske::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new SyllEsske(host, lore, general);
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

    int SyllEsske::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
