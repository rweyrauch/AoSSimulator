/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace DaughtersOfKhaine {

    constexpr std::array<int, 7> g_temple = {
            ToInteger(Temple::None),
            ToInteger(Temple::Hagg_Nar),
            ToInteger(Temple::Draichi_Ganeth),
            ToInteger(Temple::The_Kraith),
            ToInteger(Temple::Khailebron),
            ToInteger(Temple::Zainthar_Kai),
            ToInteger(Temple::Khelt_Nar)
    };

    constexpr std::array<int, 7> g_aelfCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Bathed_In_Blood),
            ToInteger(CommandTrait::Zealous_Orator),
            ToInteger(CommandTrait::Sacrificer_Overseer),
            ToInteger(CommandTrait::Terrifying_Beauty),
            ToInteger(CommandTrait::Master_Of_Poisons),
            ToInteger(CommandTrait::True_Believer)
    };

    constexpr std::array<int, 4> g_medusaCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arcane_Mastery),
            ToInteger(CommandTrait::Writhing_Coils),
            ToInteger(CommandTrait::Fearsome_Presence)
    };

    constexpr std::array<int, 4> g_melusaiCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Veteran_Of_The_Cathirar_Dhule),
            ToInteger(CommandTrait::Impenentrable_Scales),
            ToInteger(CommandTrait::Fuelled_By_Revenge)
    };

    constexpr std::array<int, 7> g_heroArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Crown_Of_Woe),
            ToInteger(Artefact::Shadracars_Fangs),
            ToInteger(Artefact::Amulet_Of_Dark_Fire),
            ToInteger(Artefact::Crone_Blade),
            ToInteger(Artefact::Thousand_And_One_Dark_Blessings),
            ToInteger(Artefact::Bloodbane_Venom)
    };

    constexpr std::array<int, 7> g_wizardArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Shadow_Stone),
            ToInteger(Artefact::Rune_Of_Ulgu),
            ToInteger(Artefact::The_Mirror_Glaive),
            ToInteger(Artefact::Sevenfold_Shadow),
            ToInteger(Artefact::Crystal_Heart),
            ToInteger(Artefact::Shade_Claw)
    };

    constexpr std::array<int, 7> g_priestArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Blood_Sigil),
            ToInteger(Artefact::Iron_Circlet),
            ToInteger(Artefact::Rune_Of_Khaine),
            ToInteger(Artefact::Crimson_Shard),
            ToInteger(Artefact::Khainite_Pendant),
            ToInteger(Artefact::Hagbrew)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Steed_Of_Shadows),
            ToInteger(Lore::Pit_Of_Shades),
            ToInteger(Lore::Mirror_Dance),
            ToInteger(Lore::The_Withering),
            ToInteger(Lore::Mindrazor),
            ToInteger(Lore::Shroud_Of_Despair)
    };

    constexpr std::array<int, 7> g_prayers = {
            ToInteger(Prayer::None),
            ToInteger(Prayer::Catechism_Of_Murder),
            ToInteger(Prayer::Blessing_Of_Khaine),
            ToInteger(Prayer::Martyrs_Sacrifice),
            ToInteger(Prayer::Crimson_Rejuvination),
            ToInteger(Prayer::Covenant_Of_The_Iron_Heart),
            ToInteger(Prayer::Sacrament_Of_Blood)
    };

} // namespace DaughtersOfKhaine
