/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Nighthaunt {

    constexpr std::array<int, 3> g_processions = {
            ToInteger(Procession::None),
            ToInteger(Procession::Emerald_Host),
            ToInteger(Procession::Reikenors_Condemned),
    };

    constexpr std::array<int, 7> g_commandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Hatred_Of_The_Living),
            ToInteger(CommandTrait::Terrifying_Entity),
            ToInteger(CommandTrait::Lingering_Spirit),
            ToInteger(CommandTrait::Spiteful_Spirit),
            ToInteger(CommandTrait::Cloaked_In_Shadow),
            ToInteger(CommandTrait::Ruler_Of_The_Spirit_Hosts)
    };

    constexpr std::array<int, 16> g_artefacts = {
            ToInteger(Artefact::None),

            ToInteger(Artefact::Shadows_Edge),
            ToInteger(Artefact::Reaper_Of_Sorrows),
            ToInteger(Artefact::Balefire_Blade),
            ToInteger(Artefact::Slitter),
            ToInteger(Artefact::Headsmans_Judgement),
            ToInteger(Artefact::Shrieking_Blade),

            ToInteger(Artefact::Cloak_Of_The_Waxing_Moon),
            ToInteger(Artefact::Pendant_Of_The_Fell_Wind),
            ToInteger(Artefact::Dreadbolt_Ring),
            ToInteger(Artefact::Mirror_Of_Screaming_Souls),
            ToInteger(Artefact::Midnight_Tome),
            ToInteger(Artefact::Covetous_Familiar),

            ToInteger(Artefact::Lightshard_Of_The_Harvest_Moon),
            ToInteger(Artefact::Wychlight_Lantern),
            ToInteger(Artefact::Beacon_Of_Nagashizzar)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Soul_Cage),
            ToInteger(Lore::Spirit_Drain),
            ToInteger(Lore::Lifestealer),
            ToInteger(Lore::Reaping_Scythe),
            ToInteger(Lore::Shademist),
            ToInteger(Lore::Spectral_Tether)
    };

} // namespace Nighthaunt
