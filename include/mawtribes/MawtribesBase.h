/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace OgorMawtribes {

    enum class Mawtribe : int {
        None = 0,
        Meatfist,
        Bloodgullet,
        Underguts,
        Boulderhead,
        Thunderbellies,
        Winterbite
    };

    enum class CommandTrait : int {
        None,

        // Tyrant,
        Furious_Guzzler,
        Prodigious_Girth,
        Killer_Reputation,      // TODO
        Mighty_Bellower,
        An_Eye_For_Loot,
        Crushing_Bulk,

        // Butcher
        Questionable_Hygiene,
        Herald_Of_The_Gulping_God,
        Growling_Stomach,
        Gastromancer,
        Rolls_Of_Fat,
        Spell_Eater,            // TODO

        // Frostlord/Huskard
        Nomadic_Raider,
        Voice_Of_The_Avalanche, // TODO
        Frostfell_Aura,         // TODO
        Master_Of_The_Mournfangs,
        Skilled_Rider,
        Touched_By_The_Everwinter,  // TODO

        // Icebrow
        Winter_Ranger,          // TODO
        Eye_Of_The_Blizzard,
        Blood_Vultures_Gaze,
        Frost_Maw,
        Raised_By_Yhetees,
        Skal_Packmaster,        // TODO

        // Tribe
        Food_For_Thought,       // Meatfist
        Nice_Drop_Of_The_Red_Stuff, // Bloodgullet TODO
        Mass_Of_Scars,          // Underguts TODO
        Lord_Of_Beasts,         // Boulderhead TODO
        Storm_Chaser,           // Thunderbellies TODO
        Wintertouched,          // Winterbite TODO
    };

    enum class Artefact : int {
        None,

        // Tyrant
        Headmasher,
        Grawls_Gut_Plate,
        Gruesome_Trophy_Rack,
        Flask_Of_Stonehorn_Blood,
        Sky_Titan_Scatter_Pistols,
        The_Fang_Of_Ghur,

        // Butcher
        Dracoline_Heart,
        Shrunken_Priest_Head,
        Wizardflesh_Apron,
        Bloodrock_Talisman,
        Grease_Smeared_Tusks,
        Rotting_Dankhold_Spores,

        // Frostlord/Huskard
        The_Rime_Shroud,
        Blade_Of_All_Frost,
        Carvalox_Flank,
        Alvagr_Rune_Token,
        Skullshards_Of_Dragaar,
        Elixir_Of_Frostwyrm,

        // Icebrow
        The_Pelt_Of_Charngar,
        Kattanak_Browplate,
        Frost_Talon_Shardbolts,
    };

    enum class Lore : int {
        None,

        // Butcher
        Fleshcrave_Curse,
        Blood_Feast,
        Ribcracker,
        Blubbergrub_Stench,
        Molten_Entrails,
        Greasy_Deluge,

        // Firebelly
        Fiery_Whirlwind,
        Billowing_Ash,
        Tongues_Of_Flame,
    };

    enum class Prayer : int {
        None,

        Pulverising_Hailstorm,
        Keening_Gale,
        Call_Of_The_Blizzard
    };

    enum class MountTrait : int {
        None,

        // Stonehorn
        Black_Clatterhorn,
        Metalcrusher,           // TODO
        Belligerent_Charger,    // TODO
        Frosthoof_Bull,
        Rockmane_Elder,
        Old_Granitetooth,       // TODO

        // Thundertusk
        Fleet_Of_Hoof,          // TODO
        Fleshgreed,
        Rimefrost_Hide,         // TODO
        Gvarnak,
        Matriarch,              // TODO
        Alvagr_Ancient          // TODO
    };

    class MawtribesBase : public Unit {
    public:

        MawtribesBase() = default;

        ~MawtribesBase() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        MawtribesBase(Mawtribe tribe, const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {
            setMawtribe(tribe);
        }

    protected:

        Mawtribe m_tribe = Mawtribe::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        mutable int m_unmodifiedChargeRoll = 0;

        void onCharged() override;

        void onStartHero(PlayerId player) override;

        int rollChargeDistance() override;

        int moveModifier() const override;

        int braveryModifier() const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int questionableHygiene(const Unit *attacker, const Weapon *weapon, const Unit *target);
        int heraldOfGulpingGod(const Unit *unit);
        int growlingStomach(const Unit *unit);
        int masterOfMournfangs(const Unit *unit);
        int raisedByYhetees(const Unit *attacker, const Model *attackingModel, const Weapon *weapon, const Unit *target);

        // Command traits
        lsignal::slot m_questionableHygiene;
        lsignal::slot m_heraldOfGulpingGod;
        lsignal::slot m_growlingStomach;
        lsignal::slot m_masterOfMournfangs;
        lsignal::slot m_raisedByYhetees;

    private:

        void setMawtribe(Mawtribe tribe);

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trampling Charge                 Yes
// Grasp of the Everwinter          Yes
// Might Make Right                 TODO
// Ravenous Brutes                  Yes
// Meatfist
//    Fleshy Stampede               Yes
//    The Unstoppable Feast         TODO
// Bloodgullet
//    Heralds of the Gulping God    Partial/TODO
//    Bloodbath                     TODO
// Underguts
//    Gunmasters                    Yes
//    Thunderous Salvo              TODO
// Boulderhead
//    Fearsome Breed                TODO
//    Deadly Hail                   TODO
//    Dig Deep Your Heels!          TODO
// Thunderbellies
//    Swift Outflank                TODO
//    Riders of the Hurricane       TODO
//    Rip and Tear                  TODO
// Winterbite
//    Ghosts in the Blizzard        TODO
//    Call of the Endless White     TODO
//    Howl of the Wind              TODO
//

    void Init();

} // namespace OgorMawtribes
