/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/VampireLord.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_basesizeWithSteed = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool VampireLord::s_registered = false;

    Unit *VampireLord::Create(const ParameterList &parameters) {
        auto legion = (CursedBloodline) GetEnumParam("Legion", parameters, g_legions[0]);
        bool withSteed = GetBoolParam("Steed", parameters, false);
        bool withWings = GetBoolParam("Wings", parameters, false);
        bool chalice = GetBoolParam("Chalice of Blood", parameters, true);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new VampireLord(legion, withSteed, withWings, chalice, lore, trait, artefact, general);
    }

    int VampireLord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void VampireLord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("Steed"),
                            BoolParameter("Wings"),
                            BoolParameter("Chalice of Blood"),
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Vampire Lord", factoryMethod);
        }
    }

    VampireLord::VampireLord(CursedBloodline legion, bool withSteed, bool withWings, bool chalice, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(legion, "Vampire Lord", 6, g_wounds, 10, 3, false, g_pointsPerUnit),
            m_blades(Weapon::Type::Melee, "Soulbound Blades", 1, 4, 3, 3, -1, RAND_D3),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Nightmare's Hooves and Teeth", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, HERO, WIZARD, VAMPIRE_LORD};
        m_weapons = {&m_blades, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        auto model = new Model(withSteed ? g_basesizeWithSteed : g_basesize, wounds());

        if (withSteed) {
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            m_move = 10;
            m_hasMount = true;
            m_hoovesAndTeeth.setMount(true);
        } else if (withWings) {
            m_move = 10;
            m_fly = true;
            m_hasMount = true;
        }

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_haveChaliceOfBlood = chalice;
    }

    void VampireLord::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) {
            deathlyInvocations(3, 12.0);

            // Chalice of Blood
            if (m_haveChaliceOfBlood && !m_usedChaliceOfBlood && remainingWounds() < wounds()) {
                heal(Dice::RollD6());
                m_usedChaliceOfBlood = true;
            }
        }
    }

    void VampireLord::onRestore() {
        SoulblightBase::onRestore();

        m_usedChaliceOfBlood = false;
    }

    void VampireLord::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
