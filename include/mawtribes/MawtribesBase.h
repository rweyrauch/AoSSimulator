/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAWTRIBESBASE_H
#define MAWTRIBESBASE_H

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
        Killer_Reputation,
        Mighty_Bellower,
        An_Eye_for_Loot,
        Crushing_Bulk,

        // Butcher
        Questionable_Hygiene,
        Herald_of_the_Gulping_God,
        Growling_Stomach,
        Gastromancer,
        Rolls_of_Fat,
        Spell_Eater,

        // Frostlord/Huskard
        Nomadic_Raider,
        Voice_of_the_Avalanche,
        Frostfell_Aura,
        Master_of_the_Mournfangs,
        Skilled_Rider,
        Touched_by_the_Everwinter,

        // Icebrow
        Winter_Rander,
        Eye_of_the_Blizzard,
        Blood_Vultures_Gaze,
        Frost_Maw,
        Raised_by_Yhetees,
        Skal_Packmaster
    };

    enum class Artefact : int {
        None,

        // Tyrant
        Headmasher,
        Grawls_Gut_Plate,
        Gruesome_Trophy_Rack,
        Flask_of_Stonehorn_Blood,
        Sky_Titan_Scatter_Pistols,
        The_Fang_of_Ghur,

        // Butcher
        Dracoline_Heart,
        Shrunken_Priest_Head,
        Wizardflesh_Apron,
        Bloodrock_Talisman,
        Grease_Smeared_Tusks,
        Rotting_Dankhold_Spores,

        // Frostlord/Huskard
        The_Rime_Shroud,
        Blade_of_All_Frost,
        Carvalox_Flank,
        Alvagr_Rune_Token,
        Skullshards_of_Dragaar,
        Elixir_of_Frostwyrm,

        // Icebrow
        The_Pelt_of_Charngar,
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
        Tongues_of_Flame,
    };

    enum class Prayer : int {
        None,

        Pulverising_Hailstorm,
        Keening_Gale,
        Call_of_the_Blizzard
    };

    enum class MountTrait : int {
        None,

        // Stonehorn
        Black_Clatterhorn,
        Metalcrusher,
        Belligerent_Charger,
        Frosthoof_Bull,
        Rockmane_Elder,
        Old_Granitetooth,

        // Thundertusk
        Fleet_of_Hoof,
        Fleshgreed,
        Rimefrost_Hide,
        Gvarnak,
        Matriarch,
        Alvagr_Ancient
    };

    class MawtribesBase : public Unit {
    public:

        MawtribesBase() = default;

        ~MawtribesBase() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setMawtribe(Mawtribe tribe);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        MawtribesBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Mawtribe m_tribe = Mawtribe::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        mutable int m_unmodifiedChargeRoll = 0;

        void onCharged() override;

        void onStartHero(PlayerId player) override;

        int rollChargeDistance() const override;

        int moveModifier() const override;

        int braveryModifier() const override;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trampling Charge                 Yes
// Grasp of the Everwinter          Yes
// Might Make Right                 TODO
// Ravenous Brutes                  Yes
// Fleshy Stampede (Meatfist)       Yes
// Heralds of the Gulping God (Bloodgullet) Partial/TODO
// Gunmasters (Underguts)           Yes
// Fearsome Breed (Boulderhead)     TODO
// Swift Outflank (Thunderbellies)  TODO
// Ghosts in the Blizzard (Winterbite)  TODO
//

    void Init();

} // namespace OgorMawtribes

#endif // MAWTRIBESBASE_H