/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace IdonethDeepkin {

    constexpr std::array<int, 7> g_enclave = {
            ToInteger(Enclave::Custom),
            ToInteger(Enclave::Ionrach),
            ToInteger(Enclave::Dhom_Hain),
            ToInteger(Enclave::Fuethan),
            ToInteger(Enclave::Morphann),
            ToInteger(Enclave::Nautilar),
            ToInteger(Enclave::Briomdar)
    };

    constexpr std::array<int, 7> g_commandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Merciless_Raider),
            ToInteger(CommandTrait::Hunter_Of_Souls),
            ToInteger(CommandTrait::Unstoppable_Fury),
            ToInteger(CommandTrait::Born_From_Agony),
            ToInteger(CommandTrait::Nightmare_Legacy),
            ToInteger(CommandTrait::Lord_Of_Storm_And_Sea)
    };

    constexpr std::array<int, 7> g_akhelianArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Sanguine_Pearl),
            ToInteger(Artefact::Potion_Of_Hateful_Frenzy),
            ToInteger(Artefact::Ankusha_Spear),
            ToInteger(Artefact::Armour_Of_The_Cythai),
            ToInteger(Artefact::Bioshock_Shell),
            ToInteger(Artefact::Abyssal_Blade)
    };

    constexpr std::array<int, 7> g_idonethArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Rune_Of_The_Surging_Tide),
            ToInteger(Artefact::Black_Pearl),
            ToInteger(Artefact::Lliandras_Last_Lament),
            ToInteger(Artefact::Terrornight_Venom),
            ToInteger(Artefact::Cloud_Of_Midnight),
            ToInteger(Artefact::Whorlshell)
    };

    constexpr std::array<int, 7> g_isharannArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Steelshell_Pearl),
            ToInteger(Artefact::Mind_Flare),
            ToInteger(Artefact::Dritchleech),
            ToInteger(Artefact::Auric_Lantern),
            ToInteger(Artefact::Disharmony_Stones),
            ToInteger(Artefact::Brain_Barnacles)
    };

    constexpr std::array<int, 7> g_arcaneArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Arcane_Pearl),
            ToInteger(Artefact::Sands_Of_Infinity),
            ToInteger(Artefact::Coral_Ring),
            ToInteger(Artefact::Bauble_Of_Buoyancy),
            ToInteger(Artefact::Kraken_Tooth),
            ToInteger(Artefact::Augury_Shells)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Steed_Of_Tides),
            ToInteger(Lore::Abyssal_Darkness),
            ToInteger(Lore::Vorpal_Maelstrom),
            ToInteger(Lore::Pressure_Of_The_Deep),
            ToInteger(Lore::Tide_Of_Fear),
            ToInteger(Lore::Arcane_Corrasion)
    };

    constexpr std::array<int, 4> g_deepmareTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Swift_Finned_Impaler),
            ToInteger(MountTrait::Savage_Ferocity),
            ToInteger(MountTrait::Voidchill_Darkness)
    };

    constexpr std::array<int, 4> g_leviadonTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Ancient),
            ToInteger(MountTrait::Denizen_Of_The_Darkest_Depths),
            ToInteger(MountTrait::Reverberating_Carapace)
    };

} // namespace IdonethDeepkin

