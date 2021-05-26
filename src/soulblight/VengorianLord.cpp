/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/VengorianLord.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 0;

    bool VengorianLord::s_registered = false;

    Unit *VengorianLord::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new VengorianLord(bloodline, lore, trait, artefact, general);
    }

    int VengorianLord::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void VengorianLord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General"),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Vengorian Lord", factoryMethod);
        }
    }

    VengorianLord::VengorianLord(CursedBloodline bloodline, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
        SoulblightBase(bloodline, "Vengorian Lord", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, MONSTER, HERO, WIZARD, VENGORIAN_LORD};
        m_weapons = {&m_sabre, &m_talons, &m_tail};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sabre);
        model->addMeleeWeapon(&m_talons);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Clotted Deluge", 6, 12,
                                                                   Attribute::Target_To_Wound_Melee, 1,
                                                                   Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void VengorianLord::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
