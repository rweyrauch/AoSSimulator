/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/AventisFirestrike.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 100;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 340;

    bool AventisFirestrike::s_registered = false;

    AventisFirestrike::AventisFirestrike() :
            StormcastEternal("Aventis Firestrike", 14, WOUNDS, 9, 3, true),
            m_staffOfHammerhal(Weapon::Type::Melee, "Staff of Hammerhal", 2, 4, 3, 3, -1, RAND_D3),
            m_hornsAndHooves(Weapon::Type::Melee, "Horns and Stamping Hooves", 1, 4, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, TAURALON, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO,
                      MONSTER, WIZARD, LORD_ARCANUM, AVENTIS_FIRESTRIKE};
        m_weapons = {&m_staffOfHammerhal, &m_hornsAndHooves};
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool AventisFirestrike::configure(Lore lore) {

        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staffOfHammerhal);
        model->addMeleeWeapon(&m_hornsAndHooves);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePyroelectricBlast(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *AventisFirestrike::Create(const ParameterList &parameters) {
        auto unit = new AventisFirestrike();
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        unit->setStormhost(Stormhost::Hammers_of_Sigmar);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AventisFirestrike::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Aventis Firestrike", factoryMethod);
        }
    }

    std::string AventisFirestrike::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int AventisFirestrike::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void AventisFirestrike::onStartCombat(PlayerId /*player*/) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    void AventisFirestrike::onCharged() {
        StormcastEternal::onCharged();

        // Meteoric Strike
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            if (Dice::rollD6() >= 2) {
                ip->applyDamage({0, 1});
            }
        }
    }

    void AventisFirestrike::onStartHero(PlayerId player) {
        StormcastEternal::onStartHero(player);

        // Thunderhead Crown
        if (player == owningPlayer()) {
            heal(1);
        }
    }

    int AventisFirestrike::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals