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
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool VampireLord::s_registered = false;

    Unit *VampireLord::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new VampireLord(bloodline, lore, trait, artefact, general);
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
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
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

    VampireLord::VampireLord(CursedBloodline bloodline, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(bloodline, "Vampire Lord", 6, g_wounds, 10, 3, true, g_pointsPerUnit),
            m_blades(Weapon::Type::Melee, "Soulbound Blades", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, HERO, WIZARD, VAMPIRE_LORD};
        m_weapons = {&m_blades};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Crimson Feast", 12, 12, GamePhase::Combat, Attribute::Attacks_Melee, 1,
                                                                                  Abilities::Target::Friendly, std::vector<Keyword>{SOULBLIGHT_GRAVELORDS, SUMMONABLE}));
    }

    void VampireLord::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(RAND_D3);

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
