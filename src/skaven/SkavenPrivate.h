/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Skaven {

    constexpr std::array<int, 7> g_masterClanCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Malevolent),
            ToInteger(CommandTrait::Verminous_Valour),
            ToInteger(CommandTrait::Savage_Overlord),
            ToInteger(CommandTrait::Supreme_Manipulator),
            ToInteger(CommandTrait::Master_Of_Magic),
            ToInteger(CommandTrait::Cunning)
    };

    constexpr std::array<int, 7> g_skryreCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Malevolent),
            ToInteger(CommandTrait::Verminous_Valour),
            ToInteger(CommandTrait::Savage_Overlord),
            ToInteger(CommandTrait::Masterful_Scavenger),
            ToInteger(CommandTrait::Deranged_Inventor),
            ToInteger(CommandTrait::Overseer_Of_Destruction)
    };

    constexpr std::array<int, 7> g_pestilensCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Malevolent),
            ToInteger(CommandTrait::Verminous_Valour),
            ToInteger(CommandTrait::Savage_Overlord),
            ToInteger(CommandTrait::Master_Of_Rot_And_Ruin),
            ToInteger(CommandTrait::Architect_Of_Death),
            ToInteger(CommandTrait::Diseased)
    };

    constexpr std::array<int, 7> g_verminousCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Malevolent),
            ToInteger(CommandTrait::Verminous_Valour),
            ToInteger(CommandTrait::Savage_Overlord),
            ToInteger(CommandTrait::Brutal_Fury),
            ToInteger(CommandTrait::Powerful),
            ToInteger(CommandTrait::Devious_Adversary)
    };

    constexpr std::array<int, 7> g_moulderCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Malevolent),
            ToInteger(CommandTrait::Verminous_Valour),
            ToInteger(CommandTrait::Savage_Overlord),
            ToInteger(CommandTrait::Moulder_Supreme),
            ToInteger(CommandTrait::Hordemaster),
            ToInteger(CommandTrait::Burly)
    };

    constexpr std::array<int, 7> g_eshinClanCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Malevolent),
            ToInteger(CommandTrait::Verminous_Valour),
            ToInteger(CommandTrait::Savage_Overlord),
            ToInteger(CommandTrait::Unrivaled_Killer),
            ToInteger(CommandTrait::Shadowmaster),
            ToInteger(CommandTrait::Incredible_Agility)
    };

    constexpr std::array<int, 7> g_masterClanArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Warpstone_Scroll),
            ToInteger(Artefact::Suspicious_Stone),
            ToInteger(Artefact::The_Gnawshard),
            ToInteger(Artefact::Skavenbrew),
            ToInteger(Artefact::Snoutgrovel_Robes),
            ToInteger(Artefact::Staff_Of_Rightful_Supremacy)
    };

    constexpr std::array<int, 7> g_skryreArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::The_Brass_Orb),
            ToInteger(Artefact::Warpstone_Armour),
            ToInteger(Artefact::Esoteric_Warp_Resonator),
            ToInteger(Artefact::Skryres_Breath_Bellows),
            ToInteger(Artefact::Vial_Of_The_Fulminator),
            ToInteger(Artefact::Vigordust_Injector)
    };

    constexpr std::array<int, 7> g_pestilensArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Blade_Of_Corruption),
            ToInteger(Artefact::The_Foul_Pendant),
            ToInteger(Artefact::Brooding_Blade),
            ToInteger(Artefact::The_Fumigatous),
            ToInteger(Artefact::Blistrevous_The_Living_Cyst),
            ToInteger(Artefact::Liber_Bubonicus)
    };

    constexpr std::array<int, 7> g_verminousArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Things_Bane),
            ToInteger(Artefact::Shield_Of_Distraction),
            ToInteger(Artefact::Screechskull_Trophies),
            ToInteger(Artefact::Flaypelt_Cloak),
            ToInteger(Artefact::Rustcursed_Armour),
            ToInteger(Artefact::Warpstone_Charm)
    };

    constexpr std::array<int, 7> g_moulderArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Lash_Of_Fangs),
            ToInteger(Artefact::Foulhide),
            ToInteger(Artefact::Snap_Snap_Snarepole),
            ToInteger(Artefact::Rat_Tail_Snake),
            ToInteger(Artefact::Rabid_Crown),
            ToInteger(Artefact::Fleshgift_Vial)
    };

    constexpr std::array<int, 7> g_eshinArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Shadow_Magnet_Trinket),
            ToInteger(Artefact::Farskitter_Cloak),
            ToInteger(Artefact::The_Three_Fangs),
            ToInteger(Artefact::Warpweeper_Stars),
            ToInteger(Artefact::The_Cube_Of_Mists),
            ToInteger(Artefact::Gnawbomb)
    };

    constexpr std::array<int, 7> g_greySeerLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Scorch),
            ToInteger(Lore::Splinter),
            ToInteger(Lore::Skitterleap),
            ToInteger(Lore::Plague),
            ToInteger(Lore::Death_Frenzy),
            ToInteger(Lore::Warpgale)
    };

    constexpr std::array<int, 4> g_skryreLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::More_More_More_Warp_Power),
            ToInteger(Lore::Chain_Warp_Lightning),
            ToInteger(Lore::Warp_Lighning_Shield)
    };

} // namespace Skaven
