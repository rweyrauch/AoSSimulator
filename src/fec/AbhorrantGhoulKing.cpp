/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/AbhorrantGhoulKing.h>
#include <spells/MysticShield.h>
#include "FleshEaterCourtsPrivate.h"
#include "SummonAbility.h"

namespace FleshEaterCourt {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 160;

    bool AbhorrantGhoulKing::s_registered = false;

    AbhorrantGhoulKing::AbhorrantGhoulKing() :
            FleshEaterCourts("Abhorrant Ghoul King", 6, g_wounds, 10, 4, false),
            m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 6, 3, 3, -1, 1) {
        m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, HERO, WIZARD,
                      ABHORRANT_GHOUL_KING};
        m_weapons = {&m_goryTalonsAndFangs};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool AbhorrantGhoulKing::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_goryTalonsAndFangs);
        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffModifierSpell>(this, "Black Hunger", 5, 24, Attribute::Attacks_Melee, 1,
                                                    Abilities::Target::SelfAndFriendly,
                                                    std::vector<Keyword>{FLESH_EATER_COURTS}));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        SummonedUnitDesc unitDesc;
        unitDesc.push_back({"Crypt Ghouls", 10});
        m_commandAbilities.push_back(
                std::make_unique<SummonAbility>(this, getRoster(), "Summon Men-at-arms", unitDesc));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *AbhorrantGhoulKing::Create(const ParameterList &parameters) {
        auto unit = new AbhorrantGhoulKing();

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);

        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == GrandCourt::None)
            unit->setCourtsOfDelusion(delusion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_abhorrantCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_abhorrantArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AbhorrantGhoulKing::Init() {
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
            s_registered = UnitFactory::Register("Abhorrant Ghoul King", factoryMethod);
        }
    }

    void AbhorrantGhoulKing::onStartHero(PlayerId player) {
        // Royal Blood
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                int woundsHealed = Dice::RollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
    }

    int AbhorrantGhoulKing::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace FleshEasterCourt
