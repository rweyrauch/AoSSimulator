/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <Board.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/CitiesOfSigmar.h"

#include "citiesofsigmar/Anointed.h"
#include "citiesofsigmar/Assassin.h"
#include "citiesofsigmar/Battlemage.h"
#include "citiesofsigmar/BattlemageOnGriffon.h"
#include "citiesofsigmar/BlackArkCorsairs.h"
#include "citiesofsigmar/BlackArkFleetmaster.h"
#include "citiesofsigmar/BlackGuard.h"
#include "citiesofsigmar/Bleakswords.h"
#include "citiesofsigmar/CelestialHurricanum.h"
#include "citiesofsigmar/Cogsmith.h"
#include "citiesofsigmar/DarkRiders.h"
#include "citiesofsigmar/Darkshards.h"
#include "citiesofsigmar/DemigryphKnights.h"
#include "citiesofsigmar/DrakespawnChariots.h"
#include "citiesofsigmar/DrakespawnKnights.h"
#include "citiesofsigmar/DreadlordOnBlackDragon.h"
#include "citiesofsigmar/Dreadspears.h"
#include "citiesofsigmar/EternalGuard.h"
#include "citiesofsigmar/Executioners.h"
#include "citiesofsigmar/Flagellants.h"
#include "citiesofsigmar/FlamespyrePhoenix.h"
#include "citiesofsigmar/FreeguildCrossbowmen.h"
#include "citiesofsigmar/FreeguildGeneral.h"
#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"
#include "citiesofsigmar/FreeguildGreatswords.h"
#include "citiesofsigmar/FreeguildGuard.h"
#include "citiesofsigmar/FreeguildHandgunners.h"
#include "citiesofsigmar/FreeguildOutriders.h"
#include "citiesofsigmar/FreeguildPistoliers.h"
#include "citiesofsigmar/FrostheartPhoenix.h"
#include "citiesofsigmar/Gyrobombers.h"
#include "citiesofsigmar/Gyrocopters.h"
#include "citiesofsigmar/Hammerers.h"
#include "citiesofsigmar/HelblasterVolleyGun.h"
#include "citiesofsigmar/HelstormRocketBattery.h"
#include "citiesofsigmar/Ironbreakers.h"
#include "citiesofsigmar/Irondrakes.h"
#include "citiesofsigmar/Kharibdyss.h"
#include "citiesofsigmar/Longbeards.h"
#include "citiesofsigmar/LuminarkOfHysh.h"
#include "citiesofsigmar/NomadPrince.h"
#include "citiesofsigmar/PhoenixGuard.h"
#include "citiesofsigmar/Runelord.h"
#include "citiesofsigmar/ScourgerunnerChariots.h"
#include "citiesofsigmar/ShadowWarriors.h"
#include "citiesofsigmar/SistersOfTheThorn.h"
#include "citiesofsigmar/SistersOfTheWatch.h"
#include "citiesofsigmar/Sorceress.h"
#include "citiesofsigmar/SorceressOnBlackDragon.h"
#include "citiesofsigmar/SteamTank.h"
#include "citiesofsigmar/WardenKing.h"
#include "citiesofsigmar/WarHydra.h"
#include "citiesofsigmar/WildRiders.h"
#include "citiesofsigmar/WildwoodRangers.h"
#include "citiesofsigmar/DoraliaVenDenst.h"
#include "citiesofsigmar/GalenVenDenst.h"
#include "CoSLore.h"


namespace CitiesOfSigmar {

    std::string CitizenOfSigmar::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "City") {
            auto cityName = magic_enum::enum_name((City) parameter.intValue);
            return std::string(cityName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
        }
        if (std::string(parameter.name) == "Narcotic") {
            auto drugName = magic_enum::enum_name((Narcotic) parameter.intValue);
            return std::string(drugName);
        }
        return ParameterValueToString(parameter);
    }

    int CitizenOfSigmar::EnumStringToInt(const std::string &enumString) {
        auto city = magic_enum::enum_cast<City>(enumString);
        if (city.has_value()) return (int) city.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto drug = magic_enum::enum_cast<Narcotic>(enumString);
        if (drug.has_value()) return (int) drug.value();

        return 0;
    }

    void CitizenOfSigmar::setCity(City city) {
        removeKeyword(HAMMERHAL);
        removeKeyword(LIVING_CITY);
        removeKeyword(GREYWATER_FASTNESS);
        removeKeyword(PHOENICIUM);
        removeKeyword(ANVILGARD);
        removeKeyword(HALLOWHEART);
        removeKeyword(TEMPESTS_EYE);
        removeKeyword(MISTHÅVN);
        removeKeyword(HAR_KURON);

        m_city = city;
        switch (city) {
            case City::Hammerhal:
                addKeyword(HAMMERHAL);
                break;
            case City::Living_City:
                addKeyword(LIVING_CITY);
                break;
            case City::Greywater_Fastness:
                addKeyword(GREYWATER_FASTNESS);
                break;
            case City::Phoenicium:
                addKeyword(PHOENICIUM);
                break;
            case City::Anvilgard:
                addKeyword(ANVILGARD);
                break;
            case City::Hallowheart:
                addKeyword(HALLOWHEART);
                break;
            case City::Tempests_Eye:
                addKeyword(TEMPESTS_EYE);
                break;
            case City::Misthåvn:
                addKeyword(MISTHÅVN);
                break;
            case City::Har_Kuron:
                addKeyword(HAR_KURON);
                break;
            default:
                break;
        }
    }

    void CitizenOfSigmar::setCommandTrait(CommandTrait trait) {
        // TODO: clear/restore previously set buff if any...

        m_commandTrait = trait;

        const Duration entireGame{GamePhase::Hero, DurationRestOfGame,  owningPlayer()};

        if (trait == CommandTrait::Personal_Levitation) {
            buffMovement(MovementRule::Can_Fly, true, entireGame);
        }
        if (trait == CommandTrait::Swift_As_The_Wind) {
            buffMovement(MovementRule::Run_And_Charge, true, entireGame);
            buffAbility(Ability::Fights_First, 1, entireGame);
        }
        if (trait == CommandTrait::Forest_Strider) {
            buffMovement(MovementRule::Run_And_Charge, true, entireGame);
        }
        if (trait == CommandTrait::Druid_Of_The_Everspring) {
            if (hasKeyword(WIZARD)) {
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Lifesurge, this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Cage_Of_Thorns, this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Ironoak_Skin, this)));
            }
            else {
                constexpr std::array<Lore, 3> g_loreOfLeaves = { Lore::Lifesurge, Lore::Cage_Of_Thorns, Lore::Ironoak_Skin };
                m_totalSpells = 1;
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(g_loreOfLeaves[Dice::RollD3()], this)));
            }
        }
        if (trait == CommandTrait::One_With_Fire_And_Ice) {
            if (hasKeyword(WIZARD)) {
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Amber_Tide, this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Phoenix_Cry, this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Golden_Mist, this)));
            }
            else {
                constexpr std::array<Lore, 3> g_loreOfThePhoenix = { Lore::Amber_Tide, Lore::Phoenix_Cry, Lore::Golden_Mist };
                m_totalSpells = 1;
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(g_loreOfThePhoenix[Dice::RollD3()], this)));
            }
        }
        if (trait == CommandTrait::Secretive_Warlock) {
            if (hasKeyword(WIZARD)) {
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Sap_Strength, this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Shadow_Daggers, this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Vitriolic_Spray, this)));
            }
            else {
                constexpr std::array<Lore, 3> g_loreOfDarkSorcery = { Lore::Sap_Strength, Lore::Shadow_Daggers, Lore::Vitriolic_Spray };
                m_totalSpells = 1;
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(g_loreOfDarkSorcery[Dice::RollD3()], this)));
            }
        }
        if ((trait == CommandTrait::Shade_Warlock) || (trait == CommandTrait::Dark_Adept)) {
            if (hasKeyword(WIZARD)) {
                m_totalSpells++;
            }
            else {
                m_totalSpells = 1;
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
                m_knownSpells.push_back(std::make_unique<MysticShield>(this));
            }
        }

    }

    void CitizenOfSigmar::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void CitizenOfSigmar::setNarcotic(Narcotic narcotic) {
        m_narcotic = narcotic;
    }

    int CitizenOfSigmar::runModifier() const {
        auto mod = Unit::runModifier();

        // Outriders of the Realm
        if (m_city == City::Tempests_Eye) mod++;

        return mod;
    }

    int CitizenOfSigmar::moveModifier() const {
        auto mod = Unit::moveModifier();

        // Alert and Forewarned
        if ((m_city == City::Tempests_Eye) && (m_battleRound == 1)) mod += 3;

        return mod;
    }

    int CitizenOfSigmar::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);

        // Alert and Forewarned
        if ((m_city == City::Tempests_Eye) && (m_battleRound == 1)) mod++;

        if (isGeneral() && (m_commandTrait == CommandTrait::Ironoak_Artisan)) {
            mod++;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Raging_Outburst) && weapon->isMelee()) {
            mod--;
        }
        return mod;
    }

    void CitizenOfSigmar::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Attuned to Nature
        if ((owningPlayer() == player) && (m_city == City::Living_City)) {
            heal(1);
        }

        // Banners Held High
        if ((owningPlayer() == player) && (m_city == City::Hammerhal)) {
            if (m_battleRound != m_bannersHeldHigh) {
                auto units = Board::Instance()->getAllUnits(owningPlayer());
                int numBanners = 0;
                for (auto unit : units) {
                    if (unit->hasKeyword(Banners)) {
                        numBanners++;
                    }
                }
                Dice::RollResult rolls;
                Dice::RollD6(numBanners, rolls);
                getRoster()->addCommandPoints(rolls.rollsGE(6));
                m_bannersHeldHigh = m_battleRound;
            }
        }

        if (owningPlayer() == player) {
            if (isGeneral() && (remainingModels() > 0) &&
                ((m_commandTrait == CommandTrait::Seat_On_The_Council) ||
                 (m_commandTrait == CommandTrait::Warden_Of_The_Flame) ||
                 (m_commandTrait == CommandTrait::Strategic_Mastermind))) {
                if (Dice::RollD6() >= 4) {
                    getRoster()->addCommandPoints(1);
                }
            }
        }

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Bathed_In_Blood)) {
                heal(1);
            }
        }
    }

    bool CitizenOfSigmar::battleshockRequired() const {
        // The Pride of Hammerhal
        if (hasKeyword(HAMMERHAL) && Board::Instance()->isUnitWithinDeploymentZone(this, owningPlayer())) {
            return false;
        }
        if (hasKeyword(PHOENICIUM)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) &&
                (general->m_commandTrait == CommandTrait::Aura_Of_Serenity) && (distanceTo(general) < 12.0)) {
                return false;
            }
        }
        if (hasKeyword(HALLOWHEART)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) &&
                (general->m_commandTrait == CommandTrait::Veteran_Of_The_Blazing_Crusade) &&
                (distanceTo(general) < 18.0)) {
                return false;
            }
        }
        return Unit::battleshockRequired();
    }

    void CitizenOfSigmar::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if ((battleRound == 1) && isGeneral() && (m_commandTrait == CommandTrait::Acadamae_Prodigy)) {
            getRoster()->addCommandPoints(1);
        }
    }

    int CitizenOfSigmar::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Acadamae_Prodigy)) {
            attacks++;
        }
        return attacks;
    }

    int CitizenOfSigmar::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        if (hasKeyword(HAMMERHAL)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && general->charged() && (general->m_commandTrait == CommandTrait::Aggressive_General) &&
                (distanceTo(general) < 12.0)) {
                mod++;
            }
        }
        if (target->hasKeyword(MONSTER) && isGeneral() && (m_commandTrait == CommandTrait::Slayer_Of_Monsters)) {
            mod++;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Raging_Outburst) && weapon->isMelee()) {
            mod++;
        }
        return mod;
    }

    Rerolls CitizenOfSigmar::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (hasKeyword(HAMMERHAL)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && general->charged() && (general->m_commandTrait == CommandTrait::Blood_Of_The_Twelve) &&
                (distanceTo(general) < 12.0)) {
                return Rerolls::Ones;
            }
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int CitizenOfSigmar::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);

        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Ironoak_Artisan)) {
            mod++;
        }
        if (target->hasKeyword(MONSTER) && isGeneral() && (m_commandTrait == CommandTrait::Slayer_Of_Monsters)) {
            mod++;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Raging_Outburst) && weapon->isMelee()) {
            mod++;
        }
        if (hasKeyword(TEMPESTS_EYE)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) && (general->m_commandTrait == CommandTrait::Hawk_Eyed) &&
                (distanceTo(general) < 12.0)) {
                mod++;
            }
        }
        return mod;
    }

    Rerolls CitizenOfSigmar::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->isMissile() && hasKeyword(GREYWATER_FASTNESS)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) && (general->m_commandTrait == CommandTrait::Drillmaster)) {
                auto units = Board::Instance()->getUnitsWithin(general, GetEnemyId(owningPlayer()), 3.0);
                if (units.empty() && (distanceTo(general) < 12.0)) {
                    return Rerolls::Ones;
                }
            }
        }
        return Unit::toHitRerolls(weapon, target);
    }

    void CitizenOfSigmar::onStartMovement(PlayerId player) {
        Unit::onStartMovement(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Ghoul_Mere_Ranger)) {
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
                for (auto unit : units) {
                    unit->buffMovement(MovementRule::Run_And_Shoot, true,
                                       {GamePhase::Shooting, m_battleRound, owningPlayer()});
                }
            }
        }
    }

    int CitizenOfSigmar::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (hasKeyword(TEMPESTS_EYE)) {
            auto general = dynamic_cast<CitizenOfSigmar *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) &&
                (general->m_commandTrait == CommandTrait::Aetherguard_Captain) && (distanceTo(general) < 12.0)) {
                mod++;
            }
        }
        return mod;
    }

    Wounds CitizenOfSigmar::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (isGeneral() && (m_commandTrait == CommandTrait::Wily_Foe)) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 6, numSixes);
        }
        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

    int CitizenOfSigmar::woundModifier() const {
        auto mod = UnitModifierInterface::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Bathed_In_Blood)) {
            mod++;
        }
        return mod;
    }

    void CitizenOfSigmar::onRestore() {
        Unit::onRestore();

        if (m_narcotic != Narcotic::None) {
            m_usedNarcotic = false;
        }
    }

    void Init() {
        Anointed::Init();
        Assassin::Init();
        Battlemage::Init();
        BattlemageOnGriffon::Init();
        BlackArkCorsairs::Init();
        BlackArkFleetmaster::Init();
        BlackGuard::Init();
        Bleakswords::Init();
        CelestialHurricanum::Init();
        Cogsmith::Init();
        DarkRiders::Init();
        Darkshards::Init();
        DemigryphKnights::Init();
        DoraliaVenDenst::Init();
        DrakespawnChariots::Init();
        DrakespawnKnights::Init();
        DreadlordOnBlackDragon::Init();
        Dreadspears::Init();
        EternalGuard::Init();
        Executioners::Init();
        Flagellants::Init();
        FlamespyrePhoenix::Init();
        FreeguildCrossbowmen::Init();
        FreeguildGeneral::Init();
        FreeguildGeneralOnGriffon::Init();
        FreeguildGreatswords::Init();
        FreeguildGuard::Init();
        FreeguildHandgunners::Init();
        FreeguildOutriders::Init();
        FreeguildPistoliers::Init();
        FrostheartPhoenix::Init();
        GalenVenDenst::Init();
        Gyrobombers::Init();
        Gyrocopters::Init();
        Hammerers::Init();
        HelblasterVolleyGun::Init();
        HelstormRocketBattery::Init();
        Ironbreakers::Init();
        Irondrakes::Init();
        Kharibdyss::Init();
        Longbeards::Init();
        LuminarkOfHysh::Init();
        NomadPrince::Init();
        PhoenixGuard::Init();
        Runelord::Init();
        ScourgerunnerChariots::Init();
        ShadowWarriors::Init();
        SistersOfTheThorn::Init();
        SistersOfTheWatch::Init();
        Sorceress::Init();
        SorceressOnBlackDragon::Init();
        SteamTank::Init();
        WardenKing::Init();
        WarHydra::Init();
        WildRiders::Init();
        WildwoodRangers::Init();
    }

} // namespace CitiesOfSigmar
