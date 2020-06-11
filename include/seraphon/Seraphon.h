/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SERAPHON_H
#define SERAPHON_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Seraphon {

    enum WayOfTheSeraphon {
        Starborne,
        Coalesced
    };

    const std::array<int, 2> g_wayOfTheSeraphon = {
            Starborne,
            Coalesced
    };

    enum Constellation {
        NoConstellation,
        KoatlsClaw,        // Coalesced
        ThunderLizard,     // Coalesced
        DracothionsTail,   // Starborne
        FangsOfSotek,      // Starborne
    };

    const std::array<int, 5> g_constellation = {
            NoConstellation,
            KoatlsClaw,        // Coalesced
            ThunderLizard,     // Coalesced
            DracothionsTail,   // Starborne
            FangsOfSotek,      // Starborne
    };

    //
    // Command Traits
    //
    enum CommandTrait {
        // Slann
        ArcaneMight,
        VastIntellect,
        GreatRememberer,

        // Saurus
        DisciplinedFury,
        ThicklyScaledHide,
        MightyWarleader,

        // Skink
        MasterOfStarRituals,
        Nimble,
        Cunning
    };

    const std::array<int, 3> g_slannCommandTrait = {
        ArcaneMight,
        VastIntellect,
        GreatRememberer
    };

    const std::array<int, 3> g_saurusCommandTrait {
        DisciplinedFury,
        ThicklyScaledHide,
        MightyWarleader
    };

    const std::array<int, 3> g_skinkCommandTrait {
        MasterOfStarRituals,
        Nimble,
        Cunning
    };

    //
    // Artefacts
    //
    enum Artefact {
        // TreasuresOfTheOldOnes
        ZoeticDial,
        LightOfDracothian,
        PrismOfAmyntok,
        ItxiGrubs,
        PlaqueOfDominion,
        ThroneOfTheLostGods,

        // CelestialRelicsOfTheWarrior
        BladeOfRealities,
        SigilsOfThePrimeHunter,
        BloodRagePendant,

        // VestmentsOfThePriesthood
        IncandescentRectrices,
        CloakOfFeathers,
        SacredStegadonHelm
    };

    const std::array<int, 6> g_treasuresOfTheOldOnes {
        ZoeticDial,
        LightOfDracothian,
        PrismOfAmyntok,
        ItxiGrubs,
        PlaqueOfDominion,
        ThroneOfTheLostGods
    };

    const std::array<int, 3> g_celestialRelicsOfTheWarrior {
        BladeOfRealities,
        SigilsOfThePrimeHunter,
        BloodRagePendant
    };

    const std::array<int, 3> g_vestmentsOfThePriesthood {
        IncandescentRectrices,
        CloakOfFeathers,
        SacredStegadonHelm
    };

    //
    // Spells
    //
    enum Lore {
        // LoreOfCelestialDomination
        CelestialApotheosis,
        WalkBetweenRealms,
        MysticalUnforging,
        CelestialEquilibrium,
        StellarTempest,
        DrainMagic,

        // LoreOfCelestialManipulation
        CelestialHarmony,
        HandOfGlory,
        ExtendAstromatrix,
        FieryConvocation,
        BindEndlessSpell,
        TideOfSerpents
    };

    const std::array<int, 6> g_loreOfCelestialDomination = {
        CelestialApotheosis,
        WalkBetweenRealms,
        MysticalUnforging,
        CelestialEquilibrium,
        StellarTempest,
        DrainMagic
    };

    const std::array<int, 6> g_loreOfCelestialManipulation = {
        CelestialHarmony,
        HandOfGlory,
        ExtendAstromatrix,
        FieryConvocation,
        BindEndlessSpell,
        TideOfSerpents
    };

    class SeraphonBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SeraphonBase() = default;

        ~SeraphonBase() override = default;

        void setWayOfTheSeraphon(WayOfTheSeraphon way, Constellation constellation);

    protected:

        SeraphonBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int moveModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        Wounds targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll,
                                          int woundRoll) const override;

        int woundModifier() const override;

    protected:

        WayOfTheSeraphon m_way = WayOfTheSeraphon::Coalesced;
        Constellation m_constellation = Constellation::ThunderLizard;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Contemplations of the Ancient Ones   TODO
// Sacred Asterisms                 TODO
//   The Great Drake                TODO
//   The Hunter's Steed             TODO
//   The Sage's Staff               TODO
// Cold Blooded                     TODO
// Predatory Fighters               Yes
// Primeval Domain                  TODO
// Scaly Skin                       Yes
// Unfeeling                        Yes
// Celestial Conjuration            TODO
// Lords of Space and Time          TODO
// Appear on Command                TODO
// First to Battle                  Yes
// Savagery Incarnate               Yes
// Mighty Beasts of War             Yes
//

    void Init();

} // namespace Seraphon

#endif //SERAPHON_H