/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef GLOOMSPITEBASE_H
#define GLOOMSPITEBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <gloomspitegitz/BadMoon.h>
#include <array>

namespace GloomspiteGitz {

    //
    // Command Traits
    //
    enum CommandTrait {
        // BlessingsOfTheBadMoon
        CunningPlans,
        FightAnotherDay,
        SneakyStabba,
        ToughNLeathery,
        DeadShouty,
        TheClammyHand,
        //GiftsOfTheGloomspite
        LowCunning,
        SpitefulGit,
        GreatShaman,
        DodgyCharacter,
        BossShaman,
        LoonTouched,

        // MarksOfTheSpiderGodsFavour
        MonstrousMount,
        MasterSpiderRider,
        UlutatingBattleCry,
        //ToughNLeathery,
        //DeadShouty,
        CreepingAssault,

        // FortuitousTroggbossTraits
        ToughAsRocks,
        AlphaTrogg,
        Loonskin,
        PulverisingGrip,
        MightyBlow,
        RealmstoneStuddedHide
    };

    const std::array<int, 6> g_blessingsOfTheBadMoon = {
        CunningPlans,
        FightAnotherDay,
        SneakyStabba,
        ToughNLeathery,
        DeadShouty,
        TheClammyHand
    };

    const std::array<int, 6> g_giftsOfTheGloomspite = {
        LowCunning,
        SpitefulGit,
        GreatShaman,
        DodgyCharacter,
        BossShaman,
        LoonTouched
    };

    const std::array<int, 6> g_marksOfTheSpiderGodsFavour = {
        MonstrousMount,
        MasterSpiderRider,
        UlutatingBattleCry,
        ToughNLeathery,
        DeadShouty,
        CreepingAssault,
    };

    const std::array<int, 6> g_fortuitousTroggbossTraits = {
        ToughAsRocks,
        AlphaTrogg,
        Loonskin,
        PulverisingGrip,
        MightyBlow,
        RealmstoneStuddedHide
    };

    //
    // Artefact
    //
    enum Artefact {
        // TroglodyticTreasures
        SpitefulProdder,
        BackstabbersBlade,
        LoonstoneTalisman,
        ThePipesOfDoom,
        TheClammyCowl,
        LeeringGitshield,

        // FoetidFetishes
        SpiteshroomFamiliar,
        MoonfaceMommet,
        StaffOfSneakyStealin,

        // VenomousValuables
        TotemOfTheSpiderGod,
        HeaddressOfManyEyes,
        TheBlackFang,
        NibblasIttyRing,
        Earskuttla,
        WebStrungCloak,

        // GlintyGubbinzThatTroggothsFound
        ShinyWotnot,
        GlowyHowzit,
        PetGribbly
    };

    const std::array<int, 6> g_troglodyticTreasures = {
        SpitefulProdder,
        BackstabbersBlade,
        LoonstoneTalisman,
        ThePipesOfDoom,
        TheClammyCowl,
        LeeringGitshield
    };

    const std::array<int, 6> g_foetidFetishes = {
        SpiteshroomFamiliar,
        MoonfaceMommet,
        StaffOfSneakyStealin
    };

    const std::array<int, 6> g_venomousValuables = {
        TotemOfTheSpiderGod,
        HeaddressOfManyEyes,
        TheBlackFang,
        NibblasIttyRing,
        Earskuttla,
        WebStrungCloak
    };

    const std::array<int, 3> g_glintyGubbinzThatTroggothsFound = {
        ShinyWotnot,
        GlowyHowzit,
        PetGribbly
    };

    class GloomspiteGitzBase : public Unit {
    public:
        GloomspiteGitzBase() = default;

        ~GloomspiteGitzBase() override = default;

    protected:

        GloomspiteGitzBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onBeginRound(int battleRound) override;

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRan() override;

    protected:

        bool inLightOfTheBadMoon() const;

        bool m_movedMoon = false;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fangz of the Bad Moon            Yes
// Bad Moon Magic                   Partial/TODO
// Loonatic Inspiration             TODO
// Lunar Squigs                     Yes
// Moonclan Fungus Brew             Yes
// Spiderfang Venom                 Yes
// Troggoth Renawal                 Yes
//

    void Init();

} // namespace GloomspiteGitz

#endif //GLOOMSPITEBASE_H