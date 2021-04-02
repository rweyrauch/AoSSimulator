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

    LordArcanumOnTauralon::LordArcanumOnTauralon(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, MountTrait mountTrait, bool isGeneral) :
            MountedStormcastEternal(stormhost, "Lord-Arcanum on Tauralon", 14, g_wounds, 9, 3, true),
            m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
            m_hornsAndHooves(Weapon::Type::Melee, "Horns and Stamping Hooves", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, TAURALON, STORMCAST_ETERNAL, SACROSANCT, HERO, MONSTER, WIZARD,
                      LORD_ARCANUM};
        m_weapons = {&m_aetherstave, &m_hornsAndHooves};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hornsAndHooves.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        m_mountTrait = mountTrait;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aetherstave);
        model->addMeleeWeapon(&m_hornsAndHooves);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLightningOrb(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Swift of Wing", 18, 18, Phase::Movement,
                                                             Attribute::Run_Distance, 2,
                                                             Abilities::Target::SelfAndFriendly,
                                                             std::vector<Keyword>(SACROSANCT)));

        m_points = g_pointsPerUnit;
    }

    Unit *LordArcanumOnTauralon::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore of the Storm", parameters, g_lore[0]);
        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int) MountTrait::None);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsOfTheTempests[0]);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordArcanumOnTauralon(stormhost, lore, trait, artefact, mountTrait, general);
    }

    void LordArcanumOnTauralon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Mount Trait", g_tauralonMountTrait[0], g_tauralonMountTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };
            s_registered = UnitFactory::Register("Lord-Arcanum on Tauralon", factoryMethod);
        }
    }

    void LordArcanumOnTauralon::onCharged() {
        MountedStormcastEternal::onCharged();

        // Meteoric Strike
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 2) {
                ip->applyDamage({0, 1}, this);
            }
        }
    }

    void LordArcanumOnTauralon::onStartCombat(PlayerId player) {
        MountedStormcastEternal::onStartCombat(player);
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int LordArcanumOnTauralon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals