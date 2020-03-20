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
#include <spells/StormcastSpells.h>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals {
    static const int BASESIZE = 100;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 320;

    bool LordArcanumOnTauralon::s_registered = false;

    LordArcanumOnTauralon::LordArcanumOnTauralon() :
            StormcastEternal("Lord-Arcanum on Tauralon", 14, WOUNDS, 9, 3, true),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_hornsAndHooves(Weapon::Type::Melee, "Horns and Stamping Hooves", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, TAURALON, STORMCAST_ETERNAL, SACROSANCT, HERO, MONSTER, WIZARD,
                      LORD_ARCANUM};
        m_weapons = {&m_aetherstave, &m_hornsAndHooves};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool LordArcanumOnTauralon::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration) {
        if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None)) {
            return false;
        }

        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_hornsAndHooves);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLightningOrb(this)));
        if (storm != LoreOfTheStorm::None)
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
        if (invigoration != LoreOfInvigoration::None)
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordArcanumOnTauralon::Create(const ParameterList &parameters) {
        auto unit = new LordArcanumOnTauralon();
        auto storm = (LoreOfTheStorm) GetEnumParam("Lore of the Storm", parameters, (int) LoreOfTheStorm::None);
        auto invigoration = (LoreOfInvigoration) GetEnumParam("Lore of Invigoration", parameters,
                                                              (int) LoreOfInvigoration::None);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, StormcastEternal::None);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(storm, invigoration);
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
                            {ParamType::Enum, "Lore of the Storm", (int) LoreOfTheStorm::None,
                             (int) LoreOfTheStorm::None, (int) LoreOfTheStorm::Stormcaller, 1},
                            {ParamType::Enum, "Lore of Invigoration", (int) LoreOfInvigoration::None,
                             (int) LoreOfInvigoration::None, (int) LoreOfInvigoration::SpeedOfLightning, 1},
                            {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None,
                             StormcastEternal::AstralTemplars, 1},
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };
            s_registered = UnitFactory::Register("Lord-Arcanum on Tauralon", factoryMethod);
        }
    }

    std::string LordArcanumOnTauralon::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Storm") {
            return ToString((LoreOfTheStorm) parameter.intValue);
        } else if (std::string(parameter.name) == "Lore of Invigoration") {
            return ToString((LoreOfInvigoration) parameter.intValue);
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int LordArcanumOnTauralon::EnumStringToInt(const std::string &enumString) {
        LoreOfTheStorm storm;
        LoreOfInvigoration invigoration;
        if (FromString(enumString, storm)) {
            return (int) storm;
        } else if (FromString(enumString, invigoration)) {
            return (int) invigoration;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void LordArcanumOnTauralon::onCharged() {
        StormcastEternal::onCharged();

        // Meteoric Strike
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto ip : units) {
            int roll = Dice::rollD6();
            if (roll >= 2) {
                ip->applyDamage({0, 1});
            }
        }
    }

    void LordArcanumOnTauralon::onStartCombat(PlayerId player) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int LordArcanumOnTauralon::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals