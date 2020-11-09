/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordArcanumOnGryphcharger.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>

#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool LordArcanumOnGryphcharger::s_registered = false;

    LordArcanumOnGryphcharger::LordArcanumOnGryphcharger() :
            StormcastEternal("Lord-Arcanum on Gryph-charger", 12, g_wounds, 9, 3, false),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD,
                      LORD_ARCANUM};
        m_weapons = {&m_aetherstave, &m_beakAndClaws};
        m_battleFieldRole = Leader;
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool LordArcanumOnGryphcharger::configure(Lore lore) {

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_beakAndClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateHealingLight(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *LordArcanumOnGryphcharger::Create(const ParameterList &parameters) {
        auto unit = new LordArcanumOnGryphcharger();
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordArcanumOnGryphcharger::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Arcanum on Gryph-charger", factoryMethod);
        }
    }

    std::string LordArcanumOnGryphcharger::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int LordArcanumOnGryphcharger::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    Wounds LordArcanumOnGryphcharger::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                   int woundRoll) const {
        // Aethereal Strike
        if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void LordArcanumOnGryphcharger::onStartCombat(PlayerId /*player*/) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int LordArcanumOnGryphcharger::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals