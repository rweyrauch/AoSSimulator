/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/StormcastEternals.h>
#include <Board.h>
#include <Roster.h>
#include "stormcast/Liberators.h"
#include "stormcast/CelestarBallista.h"
#include "stormcast/Evocators.h"
#include "stormcast/Judicators.h"
#include "stormcast/Sequitors.h"
#include "stormcast/LordOrdinator.h"
#include "stormcast/Retributors.h"
#include "stormcast/LordCelestant.h"
#include "stormcast/LordCelestantOnDracoth.h"
#include "stormcast/LordCelestantOnStardrake.h"
#include "stormcast/KnightIncantor.h"
#include "stormcast/KnightQuestor.h"
#include "stormcast/KnightZephyros.h"
#include "stormcast/VanguardHunters.h"
#include "stormcast/VanguardRaptorsHurricane.h"
#include "stormcast/VanguardRaptorsLongstrike.h"
#include "stormcast/Castigators.h"
#include "stormcast/VandusHammerhand.h"
#include "stormcast/GryphHounds.h"
#include "stormcast/Concussors.h"
#include "stormcast/Desolators.h"
#include "stormcast/Fulminators.h"
#include "stormcast/Tempestors.h"
#include "stormcast/EvocatorsOnDracolines.h"
#include "stormcast/Prosecutors.h"
#include "stormcast/Protectors.h"
#include "stormcast/Decimators.h"
#include "stormcast/NeaveBlacktalon.h"
#include "stormcast/GavrielSureheart.h"
#include "stormcast/LordExorcist.h"
#include "stormcast/LordRelictor.h"
#include "stormcast/LordArcanumOnDracoline.h"
#include "stormcast/LordArcanumOnGryphcharger.h"
#include "stormcast/LordArcanumOnTauralon.h"
#include "stormcast/LordArcanum.h"
#include "stormcast/LordAquilor.h"
#include "stormcast/LordCastellant.h"
#include "stormcast/LordVeritant.h"
#include "stormcast/KnightAzyros.h"
#include "stormcast/KnightHeraldor.h"
#include "stormcast/KnightVenator.h"
#include "stormcast/KnightVexillor.h"
#include "stormcast/AventisFirestrike.h"
#include "stormcast/AstreiaSolbright.h"
#include "stormcast/VanguardPalladors.h"
#include "stormcast/DrakeswornTemplar.h"
#include "stormcast/CelestantPrime.h"
#include "stormcast/Aetherwings.h"
#include "stormcast/SteelheartsChampions.h"
#include "stormcast/TheFarstriders.h"
#include "stormcast/StormsiresCursebreakers.h"
#include "stormcast/AveronStormsire.h"

namespace StormcastEternals {

    int StormcastEternal::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = Unit::toHitModifier(weapon, unit);

        // Beast Stalkers
        if (hasKeyword(ASTRAL_TEMPLARS) && unit->hasKeyword(MONSTER)) {
            modifier += 1;
        }

        return modifier;
    }

    Rerolls StormcastEternal::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Driven by Vengeance
        if (hasKeyword(CELESTIAL_VINDICATORS) && m_charged) {
            return RerollOnes;
        }

        return Unit::toHitRerolls(weapon, unit);
    }

    int StormcastEternal::braveryModifier() const {
        int modifier = Unit::braveryModifier();

        // First to be Forged
        if (hasKeyword(HAMMERS_OF_SIGMAR)) {
            modifier += 1;
        }

        return modifier;
    }

    Rerolls StormcastEternal::battleshockRerolls() const {
        // No True Death
        if (hasKeyword(ANVILS_OF_THE_HELDENHAMMER)) {
            return RerollFailed;
        }

        return Unit::battleshockRerolls();
    }

    void StormcastEternal::setStormhost(Stormhost host) {
        removeKeyword(HAMMERS_OF_SIGMAR);
        removeKeyword(HALLOWED_KNIGHTS);
        removeKeyword(CELESTIAL_VINDICATORS);
        removeKeyword(ANVILS_OF_THE_HELDENHAMMER);
        removeKeyword(KNIGHTS_EXCELSIOR);
        removeKeyword(CELESTIAL_WARBRINGERS);
        removeKeyword(TEMPEST_LORDS);
        removeKeyword(ASTRAL_TEMPLARS);

        m_stormHost = host;
        switch (m_stormHost) {
            case HammersOfSigmar:
                addKeyword(HAMMERS_OF_SIGMAR);
                break;
            case HallowedKnights:
                addKeyword(HALLOWED_KNIGHTS);
                break;
            case CelestialVindicators:
                addKeyword(CELESTIAL_VINDICATORS);
                break;
            case AnvilsOfTheHeldenhammer:
                addKeyword(ANVILS_OF_THE_HELDENHAMMER);
                break;
            case KnightsExcelsior:
                addKeyword(KNIGHTS_EXCELSIOR);
                break;
            case CelestialWarbringers:
                addKeyword(CELESTIAL_WARBRINGERS);
                break;
            case TempestLords:
                addKeyword(TEMPEST_LORDS);
                break;
            case AstralTemplars:
                addKeyword(ASTRAL_TEMPLARS);
                break;
            default:
                break;
        }
    }

    std::string StormcastEternal::ValueToString(const Parameter &parameter) {
        if ((std::string(parameter.name) == "Lore of the Storm") || (std::string(parameter.name) == "Lore of Invigoration") ||
            (std::string(parameter.name) == "Lore")) {
            return ToString((Lore) parameter.intValue);
        }
        if (std::string(parameter.name) == "Stormhost") {
            if (parameter.intValue == HammersOfSigmar) { return "Hammers of Sigmar"; }
            else if (parameter.intValue == HallowedKnights) { return "Hallowed Knights"; }
            else if (parameter.intValue == CelestialVindicators) { return "Celestial Vindicators"; }
            else if (parameter.intValue == AnvilsOfTheHeldenhammer) { return "Anvils of the Heldenhammer"; }
            else if (parameter.intValue == KnightsExcelsior) return "Knights Excelsior";
            else if (parameter.intValue == CelestialWarbringers) return "Celestial Warbringers";
            else if (parameter.intValue == TempestLords) return "Tempest Lords";
            else if (parameter.intValue == AstralTemplars) return "Astral Templars";
            else if (parameter.intValue == NoStormhost) { return "No Stormhost"; }
        }
        if (std::string(parameter.name) == "Command Trait") {
            if (parameter.intValue == ShieldedByFaith) return "Shielded By Faith";
            else if (parameter.intValue == ConsummateCommander) return "Consummate Commander";
            else if (parameter.intValue == CunningStrategist) return "Cunning Strategist";
            else if (parameter.intValue == ZealousCrusader) return "Zealous Crusader";
            else if (parameter.intValue == StaunchDefender) return "Staunch Defender";
            else if (parameter.intValue == ChampionOfTheRealms) return "Champion of the Realms";
        }
        if (std::string(parameter.name) == "Artefact") {
            if (parameter.intValue == StrifeEnder) return "Strife Ender";
            else if (parameter.intValue == BladeOfHeroes) return "Blade of Heroes";
            else if (parameter.intValue == HammerOfMight) return "Hammer of Might";
            else if (parameter.intValue == FangOfDracothian) return "Fang of Dracothian";
            else if (parameter.intValue == ObsidianBlade) return "Obsidian Blade";
            else if (parameter.intValue == GiftOfTheSixSmiths) return "Gift of the Six Smiths";
            else if (parameter.intValue == ArmourOfDestiny) return "Armour of Destiny";
            else if (parameter.intValue == ArmourOfSilveredSigmarite) return "Armour of Silvered Sigmarite";
            else if (parameter.intValue == DrakescaleArmour) return "Drakescale Armour";
            else if (parameter.intValue == Mirrorshield) return "Mirrorshield";
            else if (parameter.intValue == Spellshield) return "Spellshield";
            else if (parameter.intValue == FeatherfoeTorc) return "Featherfoe Torc";
            else if (parameter.intValue == TalismanOfEndurance) return "Talisman of Endurance";
            else if (parameter.intValue == ObsidianAmulet) return "Obsidian Amulet";
            else if (parameter.intValue == Luckstone) return "Luckstone";
            else if (parameter.intValue == SeedOfRebirth) return "Seed of Rebirth";
            else if (parameter.intValue == SigmaritePendant) return "Sigmarite Pendant";
            else if (parameter.intValue == QuicksilverDraught) return "Quicksilver Draught";
            else if (parameter.intValue == HurricaneStandard) return "Hurricane Standard";
            else if (parameter.intValue == LicheboneStandard) return "Lichebone Standard";
            else if (parameter.intValue == PennantOfSigmaron) return "Pennant of Sigmaron";
            else if (parameter.intValue == ShrivingLight) return "Shriving Light";
            else if (parameter.intValue == FuryBrand) return "Fury Brand";
            else if (parameter.intValue == LanternOfTheTempest) return "Lantern of the Tempest";
            else if (parameter.intValue == StaffOfFocus) return "Staff of Focus";
            else if (parameter.intValue == MindlockStaff) return "Mindlock Staff";
            else if (parameter.intValue == StaffOfAzyr) return "Staff of Azyr";
            else if (parameter.intValue == ScrollOfUnravelling) return "Scroll of Unravelling";
            else if (parameter.intValue == ScrollOfCondemnation) return "Scroll of Condemnation";
            else if (parameter.intValue == StormScroll) return "Storm Scroll";
        }
        return ParameterValueToString(parameter);
    }

    int StormcastEternal::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) return lore;

        if (enumString == "Hammers of Sigmar") { return HammersOfSigmar; }
        else if (enumString == "Hallowed Knights") { return HallowedKnights; }
        else if (enumString == "Celestial Vindicators") { return CelestialVindicators; }
        else if (enumString == "Anvils of the Heldenhammer") { return AnvilsOfTheHeldenhammer; }
        else if (enumString == "Knights Excelsior") { return KnightsExcelsior; }
        else if (enumString == "Celestial Warbringers") { return CelestialWarbringers; }
        else if (enumString == "Tempest Lords") { return TempestLords; }
        else if (enumString == "Astral Templars") { return AstralTemplars; }
        else if (enumString == "No Stormhost") { return NoStormhost; }
        else if (enumString == "Shielded By Faith") return ShieldedByFaith;
        else if (enumString == "Consummate Commander") return ConsummateCommander;
        else if (enumString == "Cunning Strategist") return CunningStrategist;
        else if (enumString == "Zealous Crusader") return ZealousCrusader;
        else if (enumString == "Staunch Defender") return StaunchDefender;
        else if (enumString == "Champion of the Realms") return ChampionOfTheRealms;
        else if (enumString == "Strife Ender") return StrifeEnder;
        else if (enumString == "Blade of Heroes") return BladeOfHeroes;
        else if (enumString == "Hammer of Might") return HammerOfMight;
        else if (enumString == "Fang of Dracothian") return FangOfDracothian;
        else if (enumString == "Obsidian Blade") return ObsidianBlade;
        else if (enumString == "Gift of the Six Smiths") return GiftOfTheSixSmiths;
        else if (enumString == "Armour of Destiny") return ArmourOfDestiny;
        else if (enumString == "Armour of Silvered Sigmarite") return ArmourOfSilveredSigmarite;
        else if (enumString == "Drakescale Armour") return DrakescaleArmour;
        else if (enumString == "Mirrorshield") return Mirrorshield;
        else if (enumString == "Spellshield") return Spellshield;
        else if (enumString == "Featherfoe Torc") return FeatherfoeTorc;
        else if (enumString == "Talisman of Endurance") return TalismanOfEndurance;
        else if (enumString == "Obsidian Amulet") return ObsidianAmulet;
        else if (enumString == "Luckstone") return Luckstone;
        else if (enumString == "Seed of Rebirth") return SeedOfRebirth;
        else if (enumString == "Sigmarite Pendant") return SigmaritePendant;
        else if (enumString == "Quicksilver Draught") return QuicksilverDraught;
        else if (enumString == "Hurricane Standard") return HurricaneStandard;
        else if (enumString == "Lichebone Standard") return LicheboneStandard;
        else if (enumString == "Pennant of Sigmaron") return PennantOfSigmaron;
        else if (enumString == "Shriving Light") return ShrivingLight;
        else if (enumString == "Fury Brand") return FuryBrand;
        else if (enumString == "Lantern of the Tempest") return LanternOfTheTempest;
        else if (enumString == "Staff of Focus") return StaffOfFocus;
        else if (enumString == "Mindlock Staff") return MindlockStaff;
        else if (enumString == "Staff of Azyr") return StaffOfAzyr;
        else if (enumString == "Scroll of Unravelling") return ScrollOfUnravelling;
        else if (enumString == "Scroll of Condemnation") return ScrollOfCondemnation;
        else if (enumString == "Storm Scroll") return StormScroll;

        return 0;
    }

    void StormcastEternal::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Grand Strategists
        if ((owningPlayer() == player) && hasKeyword(TEMPEST_LORDS)) {
            if (Dice::rollD6() >= 4) {
                m_roster->addCommandPoints(1);
            }
        }
    }

    void Init() {
        CelestantPrime::Init();
        AventisFirestrike::Init();
        AstreiaSolbright::Init();
        VandusHammerhand::Init();
        NeaveBlacktalon::Init();
        GavrielSureheart::Init();
        SteelheartsChampions::Init();
        TheFarstriders::Init();
        AveronStormsire::Init();
        StormsiresCursebreakers::Init();
        LordArcanumOnDracoline::Init();
        LordArcanumOnGryphcharger::Init();
        LordArcanumOnTauralon::Init();
        LordArcanum::Init();
        LordAquilor::Init();
        LordCelestant::Init();
        LordCelestantOnDracoth::Init();
        LordCelestantOnStardrake::Init();
        LordCastellant::Init();
        LordOrdinator::Init();
        LordExorcist::Init();
        LordRelictor::Init();
        LordVeritant::Init();
        KnightAzyros::Init();
        KnightHeraldor::Init();
        KnightQuestor::Init();
        KnightIncantor::Init();
        KnightVenator::Init();
        KnightVexillor::Init();
        KnightZephyros::Init();
        GryphHounds::Init();
        DrakeswornTemplar::Init();
        Concussors::Init();
        Desolators::Init();
        Fulminators::Init();
        Tempestors::Init();
        Prosecutors::Init();
        Decimators::Init();
        Protectors::Init();
        Retributors::Init();
        Liberators::Init();
        Judicators::Init();
        VanguardRaptorsHurricane::Init();
        VanguardRaptorsLongstrike::Init();
        VanguardHunters::Init();
        VanguardPalladors::Init();
        Aetherwings::Init();
        Castigators::Init();
        CelestarBallista::Init();
        Sequitors::Init();
        Evocators::Init();
        EvocatorsOnCelestialDracolines::Init();
    }

    bool DoSpiritFlasks(Unit *owner) {
        auto units = Board::Instance()->getUnitsWithin(owner, GetEnemyId(owner->owningPlayer()), 3.0);
        int potentialWoundsPerFlask = 0;
        for (auto ip : units) {
            if (ip->remainingModels() >= 10)
                potentialWoundsPerFlask += 3;
            else
                potentialWoundsPerFlask++;
        }

        int numFlasks = 0;
        if (potentialWoundsPerFlask > owner->remainingWounds() * 6) {
            numFlasks = 3;
        } else if (potentialWoundsPerFlask > owner->remainingWounds() * 4) {
            numFlasks = 2;
        } else if (potentialWoundsPerFlask > owner->remainingWounds() * 2) {
            numFlasks = 1;
        }

        // Going out with a bang!
        if (owner->remainingWounds() == 1) {
            numFlasks = 3;
        }

        if (numFlasks) {
            for (auto ip : units) {
                int damage = 1;
                if (ip->remainingModels() >= 10) damage = RAND_D3;

                int mortalsTarget = 0;
                for (auto i = 0; i < numFlasks; i++) {
                    mortalsTarget += Dice::rollSpecial(damage);
                }

                auto numSlain = ip->applyDamage({0, mortalsTarget});

                SimLog(Verbosity::Narrative,
                       "%s shattered %d Spirit Flasks inflicting %d mortal wounds on %s slaying %d.\n",
                       owner->name().c_str(), numFlasks, mortalsTarget, ip->name().c_str(), numSlain);
            }

            int mortalsSelf = numFlasks;
            auto dead = owner->applyDamage({0, mortalsSelf});
            SimLog(Verbosity::Narrative, "Spirit Flasks inflicted %s wounds on %s.  Slaying %d models.\n",
                   mortalsSelf, owner->name().c_str(), dead);
        }

        return (numFlasks != 0);
    }

} // namespace StormcastEternals
