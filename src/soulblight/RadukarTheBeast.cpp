/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/RadukarTheBeast.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"
#include "Roster.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 0;

    bool RadukarTheBeast::s_registered = false;

    Unit *RadukarTheBeast::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new RadukarTheBeast(general);
    }

    int RadukarTheBeast::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void RadukarTheBeast::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Radukar The Beast", factoryMethod);
        }
    }

    RadukarTheBeast::RadukarTheBeast(bool isGeneral) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Radukar The Beast", 8, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, VAMPIRE_LORD, RADUKAR_THE_BEAST};
        m_weapons = {&m_claws, &m_blade};
        m_battleFieldRole = Role::Leader;

        // Bounding Charge
        m_runAndCharge = true;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_claws);
        model->setName("Radukar The Beast");
        addModel(model);
    }

    void RadukarTheBeast::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

    Wounds RadukarTheBeast::weaponDamage(const Model *attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                         int woundRoll) const {
        // Unleashed Ferocity
        if ((weapon->name() == m_claws.name()) && (hitRoll == 6)) {
            return {0, 2, Wounds::Source::Weapon_Melee, weapon};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int RadukarTheBeast::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = SoulblightBase::targetHitModifier(weapon, attacker);
        // Supernatural Reflexes
        mod--;
        return mod;
    }

    void RadukarTheBeast::onRestore() {
        SoulblightBase::onRestore();

        m_usedMusteringHowl = false;
    }

    void RadukarTheBeast::onEndMovement(PlayerId player) {
        SoulblightBase::onEndMovement(player);

        if (!m_usedMusteringHowl && (m_roster->getCommandPoints() > 0)) {
            auto factory = UnitFactory::LookupUnit("Dire Wolves");
            if (factory) {
                if (m_roster) {
                    // Create default sized unit of 10 Dire Wolves
                    auto unit = std::shared_ptr<Unit>(UnitFactory::Create("Dire Wolves", factory->m_parameters));
                    unit->deploy(position(), orientation());
                    m_roster->addUnit(unit);
                }
                m_roster->useCommandPoint();
                m_usedMusteringHowl = true;
            }
        }
    }

} // namespace Soulblight
