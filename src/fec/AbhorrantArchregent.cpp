/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/AbhorrantArchregent.h>
#include <UnitFactory.h>
#include <Spell.h>
#include <spells/MysticShield.h>
#include "FleshEaterCourtsPrivate.h"
#include "SummonAbility.h"
#include "FeCSpells.h"

namespace FleshEaterCourt {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 240;

    bool AbhorrantArchregent::s_registered = false;

    AbhorrantArchregent::AbhorrantArchregent(GrandCourt court, Delusion delusion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            FleshEaterCourts(court, delusion, "Abhorrant Archregent", 6, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 7, 3, 3, -1, 1) {
        m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, HERO, WIZARD, ABHORRANT_ARCHREGENT};
        m_weapons = {&m_goryTalonsAndFangs};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 2;
        m_totalSpells = 2;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_goryTalonsAndFangs);
        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffModifierSpell>(this, "Ferocious Hunger", 6, 24, Attribute::Attacks_Melee, RAND_D3,
                                                    Abilities::Target::SelfAndFriendly,
                                                    std::vector<Keyword>{FLESH_EATER_COURTS}));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        SummonedUnitDesc unitDesc;
        unitDesc.push_back({"Crypt Ghouls", 20});
        unitDesc.push_back({"Crypt Flayers", 3});
        unitDesc.push_back({"Crypt Horrors", 3});
        unitDesc.push_back({"Varghulf Courtier", 1});
        unitDesc.push_back({"Crypt Ghast Courtier", 1});
        unitDesc.push_back({"Crypt Infernal Courtier", 1});
        unitDesc.push_back({"Crypt Haunter Courtier", 1});

        m_commandAbilities.push_back(
                std::make_unique<SummonAbility>(this, getRoster(), "Summon Imperial Guard", unitDesc));
    }

    Unit *AbhorrantArchregent::Create(const ParameterList &parameters) {
        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_abhorrantCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_abhorrantArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AbhorrantArchregent(court, delusion, lore, trait, artefact, general);
    }

    void AbhorrantArchregent::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                            EnumParameter("Command Trait", g_abhorrantCommandTraits[0], g_abhorrantCommandTraits),
                            EnumParameter("Artefact", g_abhorrantArtefacts[0], g_abhorrantArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Abhorrant Archregent", factoryMethod);
        }
    }

    void AbhorrantArchregent::onStartHero(PlayerId player) {
        // Imperial Blood
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                int woundsHealed = 3;
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }

    }

    int AbhorrantArchregent::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace FleshEasterCourt
