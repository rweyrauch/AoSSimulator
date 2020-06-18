/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IDONETHDEEPKINPRIVATE_H
#define IDONETHDEEPKINPRIVATE_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace IdonethDeepkin {

    constexpr std::array<int, 7> g_enclave = {
            to_integer(Enclave::Custom),
            to_integer(Enclave::Ionrach),
            to_integer(Enclave::Dhom_Hain),
            to_integer(Enclave::Fuethan),
            to_integer(Enclave::Morphann),
            to_integer(Enclave::Nautilar),
            to_integer(Enclave::Briomdar)
    };

    constexpr std::array<int, 7> g_commandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Merciless_Raider),
            to_integer(CommandTrait::Hunter_of_Souls),
            to_integer(CommandTrait::Unstoppable_Fury),
            to_integer(CommandTrait::Born_From_Agony),
            to_integer(CommandTrait::Nightmare_Legacy),
            to_integer(CommandTrait::Lord_of_Storm_and_Sea)
    };

    constexpr std::array<int, 7> g_akhelianArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Sanguine_Pearl),
            to_integer(Artefact::Potion_of_Hateful_Frenzy),
            to_integer(Artefact::Ankusha_Spear),
            to_integer(Artefact::Armour_of_the_Cythai),
            to_integer(Artefact::Bioshock_Shell),
            to_integer(Artefact::Abyssal_Blade)
    };

    constexpr std::array<int, 7> g_idonethArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Rune_of_the_Surging_Tide),
            to_integer(Artefact::Black_Pearl),
            to_integer(Artefact::Lliandras_Last_Lament),
            to_integer(Artefact::Terrornight_Venom),
            to_integer(Artefact::Cloud_of_Midnight),
            to_integer(Artefact::Whorlshell)
    };

    constexpr std::array<int, 7> g_isharannArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Steelshell_Pearl),
            to_integer(Artefact::Mind_Flare),
            to_integer(Artefact::Dritchleech),
            to_integer(Artefact::Auric_Lantern),
            to_integer(Artefact::Disharmony_Stones),
            to_integer(Artefact::Brain_Barnacles)
    };

    constexpr std::array<int, 7> g_arcaneArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Arcane_Pearl),
            to_integer(Artefact::Sands_of_Infinity),
            to_integer(Artefact::Coral_Ring),
            to_integer(Artefact::Bauble_of_Buoyancy),
            to_integer(Artefact::Kraken_Tooth),
            to_integer(Artefact::Augury_Shells)
    };

    constexpr std::array<int, 7> g_lore = {
        to_integer(Lore::None),
        to_integer(Lore::Steed_of_Tides),
        to_integer(Lore::Abyssal_Darkness),
        to_integer(Lore::Vorpal_Maelstrom),
        to_integer(Lore::Pressure_of_the_Deep),
        to_integer(Lore::Tide_of_Fear),
        to_integer(Lore::Arcane_Corrasion)
    };

} // namespace IdonethDeepkin

#endif // IDONETHDEEPKINPRIVATE_H
