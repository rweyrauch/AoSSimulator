/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/AstreiaSolbright.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool AstreiaSolbright::s_registered = false;

    AstreiaSolbright::AstreiaSolbright(Lore lore, MountTrait trait, bool isGeneral) :
            MountedStormcastEternal(Stormhost::Hammers_Of_Sigmar, "Astreia Solblight", 12, g_wounds, 9, 3, false, g_pointsPerUnit),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO,
                      WIZARD, LORD_ARCANUM, ASTREIA_SOLBRIGHT};
        m_weapons = {&m_aetherstave, &m_monstrousClaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_monstrousClaws.setMount(true);

        setGeneral(isGeneral);

        s_globalBraveryMod.connect(this, &AstreiaSolbright::supernaturalRoar, &m_connection);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        m_mountTrait = trait;
        if (m_mountTrait == MountTrait::Bounding_Leap) {
            m_pileInMove = 6;
        }

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_monstrousClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLightningPulse(this)));

        m_points = g_pointsPerUnit;
    }

    AstreiaSolbright::~AstreiaSolbright() {
        m_connection.disconnect();
    }

    Unit *AstreiaSolbright::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int) MountTrait::None);
        auto general = GetBoolParam("General", parameters, false);
        return new AstreiaSolbright(lore, trait, general);
    }

    void AstreiaSolbright::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Mount Trait", g_dracolineMountTrait[0], g_dracolineMountTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Astreia Solblight", factoryMethod);
        }
    }

    Wounds AstreiaSolbright::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Thunderous Pounce
        if (m_charged && weapon->name() == m_monstrousClaws.name()) {
            return {Dice::RollD3(), 0, Wounds::Source::Weapon_Melee, weapon};
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls AstreiaSolbright::chargeRerolls() const {
        // Thunderous Pounce
        return Rerolls::Failed;
    }

    void AstreiaSolbright::onStartCombat(PlayerId /*player*/) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int AstreiaSolbright::supernaturalRoar(const Unit *target) {
        // Supernatural Roar
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    int AstreiaSolbright::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals