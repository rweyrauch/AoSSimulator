/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAWTRIBEPRIVATE_H
#define MAWTRIBEPRIVATE_H

#include <mawtribes/MawtribesBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace OgorMawtribes {

    constexpr std::array<int, 7> g_mawtribe = {
            to_integer(Mawtribe::None),
            to_integer(Mawtribe::Meatfist),
            to_integer(Mawtribe::Bloodgullet),
            to_integer(Mawtribe::Underguts),
            to_integer(Mawtribe::Boulderhead),
            to_integer(Mawtribe::Thunderbellies),
            to_integer(Mawtribe::Winterbite)
    };

    constexpr std::array<int, 7> g_tyrantTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Furious_Guzzler),
            to_integer(CommandTrait::Prodigious_Girth),
            to_integer(CommandTrait::Killer_Reputation),
            to_integer(CommandTrait::Mighty_Bellower),
            to_integer(CommandTrait::An_Eye_for_Loot),
            to_integer(CommandTrait::Crushing_Bulk),
    };

    constexpr std::array<int, 7> g_butcherTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Questionable_Hygiene),
            to_integer(CommandTrait::Herald_of_the_Gulping_God),
            to_integer(CommandTrait::Growling_Stomach),
            to_integer(CommandTrait::Gastromancer),
            to_integer(CommandTrait::Rolls_of_Fat),
            to_integer(CommandTrait::Spell_Eater)
    };

    constexpr std::array<int, 7> g_frostlordTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Nomadic_Raider),
            to_integer(CommandTrait::Voice_of_the_Avalanche),
            to_integer(CommandTrait::Frostfell_Aura),
            to_integer(CommandTrait::Master_of_the_Mournfangs),
            to_integer(CommandTrait::Skilled_Rider),
            to_integer(CommandTrait::Touched_by_the_Everwinter)
    };

    constexpr std::array<int, 7> g_icebrowTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Winter_Rander),
            to_integer(CommandTrait::Eye_of_the_Blizzard),
            to_integer(CommandTrait::Blood_Vultures_Gaze),
            to_integer(CommandTrait::Frost_Maw),
            to_integer(CommandTrait::Raised_by_Yhetees),
            to_integer(CommandTrait::Skal_Packmaster)
    };

    constexpr std::array<int, 7> g_tyrantArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Headmasher),
            to_integer(Artefact::Grawls_Gut_Plate),
            to_integer(Artefact::Gruesome_Trophy_Rack),
            to_integer(Artefact::Flask_of_Stonehorn_Blood),
            to_integer(Artefact::Sky_Titan_Scatter_Pistols),
            to_integer(Artefact::The_Fang_of_Ghur)
    };

    constexpr std::array<int, 7> g_butcherArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Dracoline_Heart),
            to_integer(Artefact::Shrunken_Priest_Head),
            to_integer(Artefact::Wizardflesh_Apron),
            to_integer(Artefact::Bloodrock_Talisman),
            to_integer(Artefact::Grease_Smeared_Tusks),
            to_integer(Artefact::Rotting_Dankhold_Spores)
    };

    constexpr std::array<int, 7> g_frostlordArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::The_Rime_Shroud),
            to_integer(Artefact::Blade_of_All_Frost),
            to_integer(Artefact::Carvalox_Flank),
            to_integer(Artefact::Alvagr_Rune_Token),
            to_integer(Artefact::Skullshards_of_Dragaar),
            to_integer(Artefact::Elixir_of_Frostwyrm)
    };

    constexpr std::array<int, 4> g_icebrowArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::The_Pelt_of_Charngar),
            to_integer(Artefact::Kattanak_Browplate),
            to_integer(Artefact::Frost_Talon_Shardbolts)
    };

    constexpr std::array<int, 7> g_butcherLore = {
            to_integer(Lore::None),
            to_integer(Lore::Fleshcrave_Curse),
            to_integer(Lore::Blood_Feast),
            to_integer(Lore::Ribcracker),
            to_integer(Lore::Blubbergrub_Stench),
            to_integer(Lore::Molten_Entrails),
            to_integer(Lore::Greasy_Deluge)
    };

    constexpr std::array<int, 4> g_firebellyLore = {
            to_integer(Lore::None),
            to_integer(Lore::Fiery_Whirlwind),
            to_integer(Lore::Billowing_Ash),
            to_integer(Lore::Tongues_of_Flame)
    };

    constexpr std::array<int, 4> g_prayers = {
            to_integer(Prayer::None),
            to_integer(Prayer::Pulverising_Hailstorm),
            to_integer(Prayer::Keening_Gale),
            to_integer(Prayer::Call_of_the_Blizzard)
    };

    constexpr std::array<int, 7> g_stonehornTraits = {
            to_integer(MountTrait::None),
            to_integer(MountTrait::Black_Clatterhorn),
            to_integer(MountTrait::Metalcrusher),
            to_integer(MountTrait::Belligerent_Charger),
            to_integer(MountTrait::Frosthoof_Bull),
            to_integer(MountTrait::Rockmane_Elder),
            to_integer(MountTrait::Old_Granitetooth)
    };

    constexpr std::array<int, 7> g_thundertuskTraits = {
            to_integer(MountTrait::None),
            to_integer(MountTrait::Fleet_of_Hoof),
            to_integer(MountTrait::Fleshgreed),
            to_integer(MountTrait::Rimefrost_Hide),
            to_integer(MountTrait::Gvarnak),
            to_integer(MountTrait::Matriarch),
            to_integer(MountTrait::Alvagr_Ancient)
    };

} // namespace OgorMawtribes

#endif // MAWTRIBESPRIVATE_H