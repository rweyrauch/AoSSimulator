/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/LaukaVai.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 0;

    bool LaukaVai::s_registered = false;

    Unit *LaukaVai::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LaukaVai(lore, general);
    }

    int LaukaVai::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void LaukaVai::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Lauka Vai", factoryMethod);
        }
    }

    LaukaVai::LaukaVai(Lore lore, bool isGeneral) :
        SoulblightBase(CursedBloodline::Avengorii_Dynasty, "Lauka Vai Mother of Nightmares", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, AVENGORII_DYNASTY, MONSTER, HERO, WIZARD, LAUKA_VAI};
        m_weapons = {&m_rapier, &m_talons, &m_tail};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_rapier);
        model->addMeleeWeapon(&m_talons);
        model->addMeleeWeapon(&m_tail);
        model->setName("Lauka Vai");
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffMovementSpell>(this, "Death's Downpour", 8,
                                                                    12, MovementRule::Halve_Charge_Roll,
                                                                    true, Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void LaukaVai::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

    void LaukaVai::onCharged() {
        EventInterface::onCharged();
    }

} // namespace Soulblight
