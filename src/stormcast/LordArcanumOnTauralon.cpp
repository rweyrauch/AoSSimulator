/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordArcanumOnTauralon.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 100;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 280;

    bool LordArcanumOnTauralon::s_registered = false;

    LordArcanumOnTauralon::LordArcanumOnTauralon() :
            StormcastEternal("Lord-Arcanum on Tauralon", 14, g_wounds, 9, 3, true),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_hornsAndHooves(Weapon::Type::Melee, "Horns and Stamping Hooves", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, TAURALON, STORMCAST_ETERNAL, SACROSANCT, HERO, MONSTER, WIZARD,
                      LORD_ARCANUM};
        m_weapons = {&m_aetherstave, &m_hornsAndHooves};
        m_battleFieldRole = Leader;
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool LordArcanumOnTauralon::configure(Lore lore) {

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_hornsAndHooves);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLightningOrb(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *LordArcanumOnTauralon::Create(const ParameterList &parameters) {
        auto unit = new LordArcanumOnTauralon();
        auto lore = (Lore) GetEnumParam("Lore of the Storm", parameters,  g_lore[0]);

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

    void LordArcanumOnTauralon::Init() {
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
            s_registered = UnitFactory::Register("Lord-Arcanum on Tauralon", factoryMethod);
        }
    }

    std::string LordArcanumOnTauralon::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int LordArcanumOnTauralon::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void LordArcanumOnTauralon::onCharged() {
        StormcastEternal::onCharged();

        // Meteoric Strike
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 2) {
                ip->applyDamage({0, 1});
            }
        }
    }

    void LordArcanumOnTauralon::onStartCombat(PlayerId /*player*/) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int LordArcanumOnTauralon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals