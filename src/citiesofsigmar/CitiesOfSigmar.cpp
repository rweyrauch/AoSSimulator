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
        m_commandTrait = trait;
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
                 (m_commandTrait == CommandTrait::Warden_Of_The_Flame))) {
                if (Dice::RollD6() >= 4) {
                    getRoster()->addCommandPoints(1);
                }
            }
        }
    }

    bool CitizenOfSigmar::battleshockRequired() const {
        // The Pride of Hammerhal
        if (hasKeyword(HAMMERHAL) && Board::Instance()->isUnitWithinDeploymentZone(this, owningPlayer())) {
            return false;
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
        return mod;
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
