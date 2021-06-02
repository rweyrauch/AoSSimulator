/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/PrinceDuvalle.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool PrinceDuvalle::s_registered = false;

    Unit *PrinceDuvalle::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new PrinceDuvalle(bloodline, lore, general);
    }

    int PrinceDuvalle::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void PrinceDuvalle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Prince Duvalle", factoryMethod);
        }
    }

    PrinceDuvalle::PrinceDuvalle(CursedBloodline bloodline, Lore lore, bool isGeneral) :
        SoulblightBase(bloodline, "Prince Duvalle", 6, g_wounds, 10, 3, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, HERO, WIZARD, PRINCE_DUVALLE};
        m_weapons = {&m_blade};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->setName("Prince Duvalle");
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Fiendish Lure", 5, 6,
                                                                    std::vector<std::pair<Attribute, int>>{{Attribute::Target_To_Hit_Melee, 1}, {Attribute::Target_To_Hit_Missile, 1}},
                                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void PrinceDuvalle::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
