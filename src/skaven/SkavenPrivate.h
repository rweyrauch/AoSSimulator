/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKAVENPRIVATE_H
#define SKAVENPRIVATE_H

#include <skaven/Skaventide.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Skaven {

    constexpr std::array<int, 7> g_masterClanCommandTraits = {
        to_integer(CommandTrait::None),
        to_integer(CommandTrait::Malevolent),
        to_integer(CommandTrait::Verminous_Valour),
        to_integer(CommandTrait::Savage_Overlord),
        to_integer(CommandTrait::Supreme_Manipulator),
        to_integer(CommandTrait::Master_of_Magic),
        to_integer(CommandTrait::Cunning)
    };

    constexpr std::array<int, 7> g_skryreCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Malevolent),
            to_integer(CommandTrait::Verminous_Valour),
            to_integer(CommandTrait::Savage_Overlord),
            to_integer(CommandTrait::Masterful_Scavenger),
            to_integer(CommandTrait::Deranged_Inventor),
            to_integer(CommandTrait::Overseer_of_Destruction)
    };

    constexpr std::array<int, 7> g_pestilensCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Malevolent),
            to_integer(CommandTrait::Verminous_Valour),
            to_integer(CommandTrait::Savage_Overlord),
            to_integer(CommandTrait::Master_of_Rot_and_Ruin),
            to_integer(CommandTrait::Architect_of_Death),
            to_integer(CommandTrait::Diseased)
    };

    constexpr std::array<int, 7> g_verminousCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Malevolent),
            to_integer(CommandTrait::Verminous_Valour),
            to_integer(CommandTrait::Savage_Overlord),
            to_integer(CommandTrait::Brutal_Fury),
            to_integer(CommandTrait::Powerful),
            to_integer(CommandTrait::Devious_Adversary)
    };

    constexpr std::array<int, 7> g_moulderCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Malevolent),
            to_integer(CommandTrait::Verminous_Valour),
            to_integer(CommandTrait::Savage_Overlord),
            to_integer(CommandTrait::Moulder_Supreme),
            to_integer(CommandTrait::Hordemaster),
            to_integer(CommandTrait::Burly)
    };

    constexpr std::array<int, 7> g_eshinClanCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Malevolent),
            to_integer(CommandTrait::Verminous_Valour),
            to_integer(CommandTrait::Savage_Overlord),
            to_integer(CommandTrait::Unrivaled_Killer),
            to_integer(CommandTrait::Shadowmaster),
            to_integer(CommandTrait::Incredible_Agility)
    };

    constexpr std::array<int, 7> g_masterClanArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Warpstone_Scroll),
            to_integer(Artefact::Suspicious_Stone),
            to_integer(Artefact::The_Gnawshard),
            to_integer(Artefact::Skavenbrew),
            to_integer(Artefact::Snoutgrovel_Robes),
            to_integer(Artefact::Staff_of_Rightful_Supremacy)
    };

    constexpr std::array<int, 7> g_skryreArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::The_Brass_Orb),
            to_integer(Artefact::Warpstone_Armour),
            to_integer(Artefact::Esoteric_Warp_Resonator),
            to_integer(Artefact::Skryres_Breath_Bellows),
            to_integer(Artefact::Vial_of_the_Fulminator),
            to_integer(Artefact::Vigordust_Injector)
    };

    constexpr std::array<int, 7> g_pestilensArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Blade_of_Corruption),
            to_integer(Artefact::The_Foul_Pendant),
            to_integer(Artefact::Brooding_Blade),
            to_integer(Artefact::The_Fumigatous),
            to_integer(Artefact::Blistrevous_The_Living_Cyst),
            to_integer(Artefact::Liber_Bubonicus)
    };

    constexpr std::array<int, 7> g_verminousArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Things_Bane),
            to_integer(Artefact::Shield_of_Distraction),
            to_integer(Artefact::Screechskull_Trophies),
            to_integer(Artefact::Flaypelt_Cloak),
            to_integer(Artefact::Rustcursed_Armour),
            to_integer(Artefact::Warpstone_Charm)
    };

    constexpr std::array<int, 7> g_moulderArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Lash_of_Fangs),
            to_integer(Artefact::Foulhide),
            to_integer(Artefact::Snap_Snap_Snarepole),
            to_integer(Artefact::Rat_Tail_Snake),
            to_integer(Artefact::Rabid_Crown),
            to_integer(Artefact::Fleshgift_Vial)
    };

    constexpr std::array<int, 7> g_eshinArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Shadow_Magnet_Trinket),
            to_integer(Artefact::Farskitter_Cloak),
            to_integer(Artefact::The_Three_Fangs),
            to_integer(Artefact::Warpweeper_Stars),
            to_integer(Artefact::The_Cube_of_Mists),
            to_integer(Artefact::Gnawbomb)
    };

    constexpr std::array<int, 7> g_greySeerLore = {
        to_integer(Lore::None),
        to_integer(Lore::Scorch),
        to_integer(Lore::Splinter),
        to_integer(Lore::Skitterleap),
        to_integer(Lore::Plague),
        to_integer(Lore::Death_Frenzy),
        to_integer(Lore::Warpgale)
    };

    constexpr std::array<int, 4> g_skryreLore = {
            to_integer(Lore::None),
            to_integer(Lore::More_More_More_Warp_Power),
            to_integer(Lore::Chain_Warp_Lightning),
            to_integer(Lore::Warp_Lighning_Shield)
    };

} // namespace Skaven

#endif //SKAVENPRIVATE_H