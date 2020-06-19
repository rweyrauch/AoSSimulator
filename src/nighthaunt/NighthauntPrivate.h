/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NIGHTHAUNTPRIVATE_H
#define NIGHTHAUNTPRIVATE_H

#include <nighthaunt/Nighthaunt.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Nighthaunt {

    constexpr std::array<int, 7> g_commandTraits = {
        to_integer(CommandTrait::None),
        to_integer(CommandTrait::Hatred_of_the_Living),
        to_integer(CommandTrait::Terrifying_Entity),
        to_integer(CommandTrait::Lingering_Spirit),
        to_integer(CommandTrait::Spiteful_Spirit),
        to_integer(CommandTrait::Cloaked_in_Shadow),
        to_integer(CommandTrait::Ruler_of_the_Spirit_Hosts)
    };

    constexpr std::array<int, 16> g_artefacts = {
        to_integer(Artefact::None),

        to_integer(Artefact::Shadows_Edge),
        to_integer(Artefact::Reaper_of_Sorrows),
        to_integer(Artefact::Balefire_Blade),
        to_integer(Artefact::Slitter),
        to_integer(Artefact::Headsmans_Judgement),
        to_integer(Artefact::Shrieking_Blade),

        to_integer(Artefact::Cloak_of_the_Waxing_Moon),
        to_integer(Artefact::Pendant_of_the_Fell_Wind),
        to_integer(Artefact::Dreadbolt_Ring),
        to_integer(Artefact::Mirror_of_Screaming_Souls),
        to_integer(Artefact::Midnight_Tome),
        to_integer(Artefact::Covetous_Familiar),

        to_integer(Artefact::Lightshard_of_the_Harvest_Moon),
        to_integer(Artefact::Wychlight_Lantern),
        to_integer(Artefact::Beacon_of_Nagashizzar)
    };

    constexpr std::array<int, 7> g_lore = {
        to_integer(Lore::None),
        to_integer(Lore::Soul_Cage),
        to_integer(Lore::Spirit_Drain),
        to_integer(Lore::Lifestealer),
        to_integer(Lore::Reaping_Scythe),
        to_integer(Lore::Shademist),
        to_integer(Lore::Spectral_Tether)
    };

} // namespace Nighthaunt

#endif // NIGHTHAUNT_H