/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/RadukarTheWolf.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 135;

    bool RadukarTheWolf::s_registered = false;

    Unit *RadukarTheWolf::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new RadukarTheWolf(lore, general);
    }

    int RadukarTheWolf::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void RadukarTheWolf::Init() {
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
            s_registered = UnitFactory::Register("Radukar The Wolf", factoryMethod);
        }
    }

    RadukarTheWolf::RadukarTheWolf(Lore lore, bool isGeneral) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Radukar The Wolf", 5, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, WIZARD, VAMPIRE_LORD, RADUKAR_THE_WOLF};
        m_weapons = {&m_blade};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->setName("Radukar The Wolf");
        addModel(model);
    }

    void RadukarTheWolf::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

    Wounds RadukarTheWolf::weaponDamage(const Model *attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                        int woundRoll) const {
        // Supernatural Strength
        if (weapon->isMelee() && (woundRoll == 6)) {
            return {weapon->damage(), 1, Wounds::Source::Weapon_Melee, weapon};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Wounds RadukarTheWolf::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SoulblightBase::applyWoundSave(wounds, attackingUnit);
        auto summonables = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        const Wounds OneMortalWound{0, 1, wounds.source, wounds.sourceObject};
        const Wounds OneNormalWound{1, 0, wounds.source, wounds.sourceObject};
        for (auto unit : summonables) {
            if (unit->hasKeyword(KOSARGI_NIGHTGUARD)) {
                while ((unit->remainingModels() > 0) && (totalWounds.mortal > 0)) {
                    if (Dice::RollD6() >= 2) {
                        unit->applyDamage(OneMortalWound, this);
                        totalWounds.mortal--;
                    }
                }
                while ((unit->remainingModels() > 0) && (totalWounds.normal > 0)) {
                    if (Dice::RollD6() >= 2) {
                        unit->applyDamage(OneNormalWound, this);
                        totalWounds.normal--;
                    }
                }
            }
        }
        return totalWounds;
    }

} // namespace Soulblight
