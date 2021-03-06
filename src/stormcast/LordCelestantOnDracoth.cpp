/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnDracoth.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool LordCelestantOnDracoth::s_registered = false;

    LordCelestantOnDracoth::LordCelestantOnDracoth(Stormhost stormhost, WeaponOption weapons, bool sigmariteThundershield, CommandTrait trait, Artefact artefact, MountTrait mountTrait, bool isGeneral) :
            MountedStormcastEternal(stormhost, "Lord-Celestant on Dracoth", 10, g_wounds, 9, 3, false, g_pointsPerUnit),
            m_stormstrikeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 4, 3, 4, -1, 1),
            m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
            m_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 3, 3, -1, 2),
            m_tempestosHammer(Weapon::Type::Melee, "Tempestos Hammer", 2, 3, 3, 2, -1, RAND_D3),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, LORD_CELESTANT};
        m_weapons = {&m_stormstrikeGlaive, &m_lightningHammer, &m_thunderaxe, &m_tempestosHammer, &m_clawsAndFangs};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_clawsAndFangs.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_weapon = weapons;
        m_sigmariteThundershield = sigmariteThundershield;
        m_mountTrait = mountTrait;

        auto model = new Model(g_basesize, wounds());
        if (m_weapon == Stormstrike_Glaive) {
            model->addMeleeWeapon(&m_stormstrikeGlaive);
        } else if (m_weapon == Lightning_Hammer) {
            model->addMeleeWeapon(&m_lightningHammer);
        } else if (m_weapon == Thunderaxe) {
            model->addMeleeWeapon(&m_thunderaxe);
        } else if (m_weapon == Tempestos_Hammer) {
            model->addMeleeWeapon(&m_tempestosHammer);
        }

        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffAbilityCommandAbility>(this, "Lord of the Host", 24, 24, GamePhase::Battleshock,
                                                            Ability::Ignore_Battleshock, 1,
                                                            Abilities::Target::SelfAndFriendly,
                                                            std::vector<Keyword>(STORMCAST_ETERNAL)));
        m_points = g_pointsPerUnit;
    }

    Unit *LordCelestantOnDracoth::Create(const ParameterList &parameters) {
        auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, Lightning_Hammer);
        bool sigmariteThundershield = GetBoolParam("Sigmarite Thundershield", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int) MountTrait::None);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto general = GetBoolParam("General", parameters, false);
        return  new LordCelestantOnDracoth(stormhost, weapons, sigmariteThundershield, trait, artefact, mountTrait, general);
    }

    void LordCelestantOnDracoth::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Tempestos_Hammer, Thunderaxe, Lightning_Hammer,
                                                       Stormstrike_Glaive};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Tempestos_Hammer, weapons),
                            BoolParameter("Sigmarite Thundershield"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Mount Trait", g_dracothMountTrait[0], g_dracothMountTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };
            s_registered = UnitFactory::Register("Lord-Celestant on Dracoth", factoryMethod);
        }
    }

    std::string LordCelestantOnDracoth::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Tempestos_Hammer) {
                return "Tempestos Hammer";
            } else if (parameter.intValue == Thunderaxe) {
                return "Thunderaxe";
            } else if (parameter.intValue == Lightning_Hammer) {
                return "Lightning Hammer";
            } else if (parameter.intValue == Stormstrike_Glaive) {
                return "Stormstrike Glaive";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int
    LordCelestantOnDracoth::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = MountedStormcastEternal::extraAttacks(attackingModel, weapon, target);

        // Tempestos Hammer
        if (m_charged && weapon->name() == m_tempestosHammer.name()) {
            attacks += Dice::RollD3();
        }

        // Thunderaxe
        if ((weapon->name() == m_thunderaxe.name())) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
            for (auto unit : units) {
                if (unit->hasKeyword(STORMCAST_ETERNAL)) attacks++;
            }
        }

        return attacks;
    }

    Rerolls LordCelestantOnDracoth::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Sigmarite Thundershield
        if (m_sigmariteThundershield) {
            return Rerolls::Ones;
        }
        return MountedStormcastEternal::toSaveRerolls(weapon, attacker);
    }

    Wounds LordCelestantOnDracoth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = MountedStormcastEternal::computeReturnedDamage(weapon, saveRoll);
        // Sigmarite Thundershield
        if (m_sigmariteThundershield) {
            // 1 mortal wound for each save of a 6
            if (saveRoll == 6) {
                wounds += {0, 1};
            }
        }
        return wounds;
    }

    int LordCelestantOnDracoth::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Tempestos Hammer") {
            return Tempestos_Hammer;
        } else if (enumString == "Thunderaxe") {
            return Thunderaxe;
        } else if (enumString == "Lightning Hammer") {
            return Lightning_Hammer;
        } else if (enumString == "Stormstrike Glaive") {
            return Stormstrike_Glaive;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    Wounds
    LordCelestantOnDracoth::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((woundRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            // D6 instead of 1
            return {Dice::RollD6(), 0};
        }

        // Lightning Hammer
        if ((hitRoll == 6) && (weapon->name() == m_lightningHammer.name())) {
            return {weapon->damage(), 2};
        }

        if (m_charged && (m_weapon == Stormstrike_Glaive)) {
            return {weapon->damage() + 2, 0};
        }

        return MountedStormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int LordCelestantOnDracoth::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void LordCelestantOnDracoth::onStartShooting(PlayerId player) {
        MountedStormcastEternal::onStartShooting(player);

        // Storm Breath
        if (owningPlayer() == player) {
            auto range = (m_mountTrait == MountTrait::Thunder_Caller) ? 16.0 : 12.0;
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= range)) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
                auto units = Board::Instance()->getUnitsWithin(unit, GetEnemyId(owningPlayer()), 2.0);
                for (auto target : units) {
                    if (Dice::RollD6() >= 4) {
                        target->applyDamage({0, Dice::RollD3()}, this);
                    }
                }
            }
        }
    }

} // namespace StormcastEternals