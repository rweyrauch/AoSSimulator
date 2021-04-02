/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordArcanumOnDracoline.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 210;

    bool LordArcanumOnDracoline::s_registered = false;

    LordArcanumOnDracoline::LordArcanumOnDracoline(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, MountTrait mountTrait, bool isGeneral) :
            MountedStormcastEternal(stormhost, "Lord-Arcanum on Celestial Dracoline", 12, g_wounds, 9, 3, false),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, LORD_ARCANUM};
        m_weapons = {&m_aetherstave, &m_monstrousClaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_monstrousClaws.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalBraveryMod.connect(this, &LordArcanumOnDracoline::supernaturalRoar, &m_connection);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        m_mountTrait = mountTrait;
        if (m_mountTrait == MountTrait::Bounding_Leap) {
            m_pileInMove = 6;
        }

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_monstrousClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateStormLance(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    LordArcanumOnDracoline::~LordArcanumOnDracoline() {
        m_connection.disconnect();
    }

    Unit *LordArcanumOnDracoline::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int) MountTrait::None);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsOfTheTempests[0]);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordArcanumOnDracoline(stormhost, lore, trait, artefact, mountTrait, general);
    }

    void LordArcanumOnDracoline::Init() {
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
                            EnumParameter("Mount Trait", g_dracolineMountTrait[0], g_dracolineMountTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Arcanum on Celestial Dracoline", factoryMethod);
        }
    }


    Wounds
    LordArcanumOnDracoline::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Thunderous Pounce
        if (m_charged && weapon->name() == m_monstrousClaws.name()) {
            return {Dice::RollD3(), 0};
        }
        return MountedStormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls LordArcanumOnDracoline::chargeRerolls() const {
        // Thunderous Pounce
        return Rerolls::Failed;
    }

    void LordArcanumOnDracoline::onStartCombat(PlayerId player) {
        MountedStormcastEternal::onStartCombat(player);
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int LordArcanumOnDracoline::supernaturalRoar(const Unit *target) {
        // Supernatural Roar
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    int LordArcanumOnDracoline::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals