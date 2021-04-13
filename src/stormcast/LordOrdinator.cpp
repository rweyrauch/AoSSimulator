/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordOrdinator.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool LordOrdinator::s_registered = false;

    LordOrdinator::LordOrdinator(Stormhost stormhost, WeaponOption weaponOption, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Lord-Ordinator", 5, g_wounds, 9, 4, false),
            m_astralHammers(Weapon::Type::Melee, "Astral Hammers", 1, 6, 4, 3, 0, 1),
            m_astralGrandhammer(Weapon::Type::Melee, "Astral Grandhammer", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, LORD_ORDINATOR};
        m_weapons = {&m_astralHammers, &m_astralGrandhammer};
        m_battleFieldRole = Role::Leader;

        m_weaponOption = weaponOption;

        setStormhost(stormhost);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        if (m_weaponOption == Astral_Hammers) {
            model->addMeleeWeapon(&m_astralHammers);
        } else {
            model->addMeleeWeapon(&m_astralGrandhammer);
        }
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffAbilityCommandAbility>(this, "Solemn Duty", 18, 18, Phase::Battleshock,
                                                            Ability::Ignore_Battleshock, 1,
                                                            Abilities::Target::SelfAndFriendly,
                                                            std::vector<Keyword>(STORMCAST_ETERNAL)));

        m_points = g_pointsPerUnit;

        s_globalToHitMod.connect(this, &LordOrdinator::arcaneEngineer, &m_connection);
    }

    LordOrdinator::~LordOrdinator() {
        m_connection.disconnect();
    }

    Unit *LordOrdinator::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, false);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new LordOrdinator(stormhost, weapons, trait, artefact, general);
    }

    void LordOrdinator::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Astral_Hammers, Astral_Grandhammer};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Astral_Hammers, weapons),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Ordinator", factoryMethod);
        }
    }

    std::string LordOrdinator::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Astral_Hammers) {
                return "Astral Hammers";
            } else if (parameter.intValue == Astral_Grandhammer) {
                return "Astral Grandhammer";
            }
        }

        return StormcastEternal::ValueToString(parameter);
    }

    int LordOrdinator::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Astral Hammers") {
            return Astral_Hammers;
        } else if (enumString == "Astral Grandhammer") {
            return Astral_Grandhammer;
        }

        return StormcastEternal::EnumStringToInt(enumString);
    }

    Wounds LordOrdinator::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Comet Strike
        if (hitRoll == 6 && weapon->name() == m_astralGrandhammer.name()) {
            return {0, 2};
        }
        if (hitRoll == 6 && weapon->name() == m_astralHammers.name()) {
            // remember this
            m_meteoricSlam.push_back(target);
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void LordOrdinator::onStartCombat(PlayerId player) {
        StormcastEternal::onStartCombat(player);

        m_meteoricSlam.clear();
    }

    void LordOrdinator::onEndCombat(PlayerId player) {
        StormcastEternal::onEndCombat(player);

        // Meteoric Slam
        if (m_meteoricSlam.size() > 1) {
            // TODO: assuming all targets are the same unit and that
            // the only unit the LO hit was the target unit
            if (m_meteoricSlam.front() == m_meleeTarget) {
                Wounds slamWounds = {0, Dice::RollD3()};
                m_meleeTarget->applyDamage(slamWounds, this);
            }
        }
    }

    int LordOrdinator::arcaneEngineer(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *target) {
        if (target->hasKeyword(ORDER) && target->hasKeyword(WAR_MACHINE) &&
            (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 9.0)) {
            return 1;
        }

        return 0;
    }

    int LordOrdinator::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals