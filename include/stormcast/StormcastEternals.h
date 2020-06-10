/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMCASTETERNALS_H
#define STORMCASTETERNALS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace StormcastEternals {

    //
    // Command Traits
    //
    enum CommandTrait {
        // AspectsOfAzyr
        ShieldedByFaith,
        ConsummateCommander,
        CunningStrategist,
        ZealousCrusader,
        StaunchDefender,
        ChampionOfTheRealms
    };

    //
    // Artefacts
    //
    enum Artefact {
        // StormForgedWeapons
        StrifeEnder,
        BladeOfHeroes,
        HammerOfMight,
        FangOfDracothian,
        ObsidianBlade,
        GiftOfTheSixSmiths,

        // HeavenWroughtArmour
        ArmourOfDestiny,
        ArmourOfSilveredSigmarite,
        DrakescaleArmour,
        Mirrorshield,
        Spellshield,
        FeatherfoeTorc,

        // ArtefactsOfTheTempests
        TalismanOfEndurance,
        ObsidianAmulet,
        Luckstone,
        SeedOfRebirth,
        SigmaritePendant,
        QuicksilverDraught,

        // TreasuredStandards
        HurricaneStandard,
        LicheboneStandard,
        PennantOfSigmaron,

        // MysticLights
        ShrivingLight,
        FuryBrand,
        LanternOfTheTempest,

        // CelestialStaves
        StaffOfFocus,
        MindlockStaff,
        StaffOfAzyr,

        // ScrollsOfPower
        ScrollOfUnravelling,
        ScrollOfCondemnation,
        StormScroll
    };

    const std::array<int, 6> g_stormForgedWeapons = {
        StrifeEnder,
        BladeOfHeroes,
        HammerOfMight,
        FangOfDracothian,
        ObsidianBlade,
        GiftOfTheSixSmiths
    };

    const std::array<int, 6> g_heavenWroughtArmour = {
        ArmourOfDestiny,
        ArmourOfSilveredSigmarite,
        DrakescaleArmour,
        Mirrorshield,
        Spellshield,
        FeatherfoeTorc
    };

    const std::array<int, 6> g_artefactsOfTheTempests = {
        TalismanOfEndurance,
        ObsidianAmulet,
        Luckstone,
        SeedOfRebirth,
        SigmaritePendant,
        QuicksilverDraught
    };

    const std::array<int, 3> g_treasuredStandards = {
        HurricaneStandard,
        LicheboneStandard,
        PennantOfSigmaron
    };

    const std::array<int, 3> g_mysticLights = {
        ShrivingLight,
        FuryBrand,
        LanternOfTheTempest
    };

    const std::array<int, 3> g_celestialStaves = {
        StaffOfFocus,
        MindlockStaff,
        StaffOfAzyr
    };

    const std::array<int, 3> g_scrollsOfPower = {
        ScrollOfUnravelling,
        ScrollOfCondemnation,
        StormScroll
    };

    enum Stormhost {
        NoStormhost = 0,
        HammersOfSigmar,
        HallowedKnights,
        CelestialVindicators,
        AnvilsOfTheHeldenhammer,
        KnightsExcelsior,
        CelestialWarbringers,
        TempestLords,
        AstralTemplars,
    };

    const std::array<int, 9> g_stormhost = {
        NoStormhost,
        HammersOfSigmar,
        HallowedKnights,
        CelestialVindicators,
        AnvilsOfTheHeldenhammer,
        KnightsExcelsior,
        CelestialWarbringers,
        TempestLords,
        AstralTemplars,
    };

    class StormcastEternal : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        StormcastEternal() = default;

        ~StormcastEternal() override = default;

        void setStormhost(Stormhost host);

    protected:
        StormcastEternal(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int braveryModifier() const override;

        Rerolls battleshockRerolls() const override;

        void onStartHero(PlayerId player) override;

    protected:

        Stormhost m_stormHost = NoStormhost;

    };

    void Init();

    bool DoSpiritFlasks(Unit *owner);

} // namespace StormcastEternals

#endif //STORMCASTETERNALS_H