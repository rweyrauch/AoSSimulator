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

    AstreiaSolbright::AstreiaSolbright() :
            MountedStormcastEternal("Astreia Solblight", 12, g_wounds, 9, 3, false),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO,
                      WIZARD, LORD_ARCANUM, ASTREIA_SOLBRIGHT};
        m_weapons = {&m_aetherstave, &m_monstrousClaws};
        m_battleFieldRole = Leader;
        m_hasMount = true;
        m_monstrousClaws.setMount(true);

        s_globalBraveryMod.connect(this, &AstreiaSolbright::supernaturalRoar, &m_connection);

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    AstreiaSolbright::~AstreiaSolbright() {
        m_connection.disconnect();
    }

    bool AstreiaSolbright::configure(Lore lore, MountTrait trait) {

        m_mountTrait = trait;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_monstrousClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLightningPulse(this)));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *AstreiaSolbright::Create(const ParameterList &parameters) {
        auto unit = new AstreiaSolbright();
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int)MountTrait::None);

        unit->setStormhost(Stormhost::Hammers_Of_Sigmar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore, trait);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Wounds AstreiaSolbright::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Thunderous Pounce
        if (m_charged && weapon->name() == m_monstrousClaws.name()) {
            return {Dice::RollD3(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls AstreiaSolbright::chargeRerolls() const {
        // Thunderous Pounce
        return Reroll_Failed;
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

    int AstreiaSolbright::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals