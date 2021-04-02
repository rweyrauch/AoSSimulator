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
#include <Roster.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 100;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 300;

    bool AventisFirestrike::s_registered = false;

    AventisFirestrike::AventisFirestrike(Lore lore, MountTrait trait, bool isGeneral) :
            MountedStormcastEternal(Stormhost::Hammers_Of_Sigmar, "Aventis Firestrike", 14, g_wounds, 9, 3, true),
            m_staffOfHammerhal(Weapon::Type::Melee, "Staff of Hammerhal", 2, 4, 3, 3, -1, RAND_D3),
            m_hornsAndHooves(Weapon::Type::Melee, "Horns and Stamping Hooves", 1, 4, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, TAURALON, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO,
                      MONSTER, WIZARD, LORD_ARCANUM, AVENTIS_FIRESTRIKE};
        m_weapons = {&m_staffOfHammerhal, &m_hornsAndHooves};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_hornsAndHooves.setMount(true);

        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        m_mountTrait = trait;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staffOfHammerhal);
        model->addMeleeWeapon(&m_hornsAndHooves);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePyroelectricBlast(this)));

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Fiery Orator", 12, 12, Phase::Combat,
                                                             Attribute::To_Wound_Melee, 1,
                                                             Abilities::Target::Friendly));
        m_points = g_pointsPerUnit;
    }

    Unit *AventisFirestrike::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int) MountTrait::None);
        auto general = GetBoolParam("General", parameters, false);
        return new AventisFirestrike(lore, trait, general);
    }

    void AventisFirestrike::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Mount Trait", g_tauralonMountTrait[0], g_tauralonMountTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Aventis Firestrike", factoryMethod);
        }
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
            if (Dice::RollD6() >= 2) {
                ip->applyDamage({0, 1}, this);
            }
        }
    }

    void AventisFirestrike::onStartHero(PlayerId player) {
        StormcastEternal::onStartHero(player);

        // Thunderhead Crown
        if (player == owningPlayer()) {
            heal(1);
        }

        // The Magister of Hammerhal
        if (m_battleRound == 1) {
            if (isGeneral() && (hasKeyword(HAMMERHAL))) {
                getRoster()->addCommandPoints(1);
            }
        }
    }

    int AventisFirestrike::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals