/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace OgorMawtribes {

    constexpr std::array<int, 7> g_mawtribe = {
            ToInteger(Mawtribe::None),
            ToInteger(Mawtribe::Meatfist),
            ToInteger(Mawtribe::Bloodgullet),
            ToInteger(Mawtribe::Underguts),
            ToInteger(Mawtribe::Boulderhead),
            ToInteger(Mawtribe::Thunderbellies),
            ToInteger(Mawtribe::Winterbite)
    };

    constexpr std::array<int, 7> g_tyrantTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Furious_Guzzler),
            ToInteger(CommandTrait::Prodigious_Girth),
            ToInteger(CommandTrait::Killer_Reputation),
            ToInteger(CommandTrait::Mighty_Bellower),
            ToInteger(CommandTrait::An_Eye_For_Loot),
            ToInteger(CommandTrait::Crushing_Bulk),
    };

    constexpr std::array<int, 7> g_butcherTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Questionable_Hygiene),
            ToInteger(CommandTrait::Herald_Of_The_Gulping_God),
            ToInteger(CommandTrait::Growling_Stomach),
            ToInteger(CommandTrait::Gastromancer),
            ToInteger(CommandTrait::Rolls_Of_Fat),
            ToInteger(CommandTrait::Spell_Eater)
    };

    constexpr std::array<int, 7> g_frostlordTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Nomadic_Raider),
            ToInteger(CommandTrait::Voice_Of_The_Avalanche),
            ToInteger(CommandTrait::Frostfell_Aura),
            ToInteger(CommandTrait::Master_Of_The_Mournfangs),
            ToInteger(CommandTrait::Skilled_Rider),
            ToInteger(CommandTrait::Touched_By_The_Everwinter)
    };

    constexpr std::array<int, 7> g_icebrowTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Winter_Ranger),
            ToInteger(CommandTrait::Eye_Of_The_Blizzard),
            ToInteger(CommandTrait::Blood_Vultures_Gaze),
            ToInteger(CommandTrait::Frost_Maw),
            ToInteger(CommandTrait::Raised_By_Yhetees),
            ToInteger(CommandTrait::Skal_Packmaster)
    };

    constexpr std::array<int, 7> g_tyrantArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Headmasher),
            ToInteger(Artefact::Grawls_Gut_Plate),
            ToInteger(Artefact::Gruesome_Trophy_Rack),
            ToInteger(Artefact::Flask_Of_Stonehorn_Blood),
            ToInteger(Artefact::Sky_Titan_Scatter_Pistols),
            ToInteger(Artefact::The_Fang_Of_Ghur)
    };

    constexpr std::array<int, 7> g_butcherArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Dracoline_Heart),
            ToInteger(Artefact::Shrunken_Priest_Head),
            ToInteger(Artefact::Wizardflesh_Apron),
            ToInteger(Artefact::Bloodrock_Talisman),
            ToInteger(Artefact::Grease_Smeared_Tusks),
            ToInteger(Artefact::Rotting_Dankhold_Spores)
    };

    constexpr std::array<int, 7> g_frostlordArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::The_Rime_Shroud),
            ToInteger(Artefact::Blade_Of_All_Frost),
            ToInteger(Artefact::Carvalox_Flank),
            ToInteger(Artefact::Alvagr_Rune_Token),
            ToInteger(Artefact::Skullshards_Of_Dragaar),
            ToInteger(Artefact::Elixir_Of_Frostwyrm)
    };

    constexpr std::array<int, 4> g_icebrowArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::The_Pelt_Of_Charngar),
            ToInteger(Artefact::Kattanak_Browplate),
            ToInteger(Artefact::Frost_Talon_Shardbolts)
    };

    constexpr std::array<int, 7> g_butcherLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Fleshcrave_Curse),
            ToInteger(Lore::Blood_Feast),
            ToInteger(Lore::Ribcracker),
            ToInteger(Lore::Blubbergrub_Stench),
            ToInteger(Lore::Molten_Entrails),
            ToInteger(Lore::Greasy_Deluge)
    };

    constexpr std::array<int, 4> g_firebellyLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Fiery_Whirlwind),
            ToInteger(Lore::Billowing_Ash),
            ToInteger(Lore::Tongues_Of_Flame)
    };

    constexpr std::array<int, 4> g_prayers = {
            ToInteger(Prayer::None),
            ToInteger(Prayer::Pulverising_Hailstorm),
            ToInteger(Prayer::Keening_Gale),
            ToInteger(Prayer::Call_Of_The_Blizzard)
    };

    constexpr std::array<int, 7> g_stonehornTraits = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Black_Clatterhorn),
            ToInteger(MountTrait::Metalcrusher),
            ToInteger(MountTrait::Belligerent_Charger),
            ToInteger(MountTrait::Frosthoof_Bull),
            ToInteger(MountTrait::Rockmane_Elder),
            ToInteger(MountTrait::Old_Granitetooth)
    };

    constexpr std::array<int, 7> g_thundertuskTraits = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Fleet_Of_Hoof),
            ToInteger(MountTrait::Fleshgreed),
            ToInteger(MountTrait::Rimefrost_Hide),
            ToInteger(MountTrait::Gvarnak),
            ToInteger(MountTrait::Matriarch),
            ToInteger(MountTrait::Alvagr_Ancient)
    };

} // namespace OgorMawtribes
