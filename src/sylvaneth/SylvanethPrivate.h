/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Sylvaneth {

    constexpr std::array<int, 8> g_glade = {
            ToInteger(Glade::None),
            ToInteger(Glade::Oakenbrow),
            ToInteger(Glade::Gnarlroot),
            ToInteger(Glade::Heartwood),
            ToInteger(Glade::Ironbark),
            ToInteger(Glade::Winterleaf),
            ToInteger(Glade::Dreadwood),
            ToInteger(Glade::Harvestboon)
    };

    constexpr std::array<int, 7> g_aspectsOfWar = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Dread_Harvester),
            ToInteger(CommandTrait::Gnarled_Warrior),
            ToInteger(CommandTrait::Gift_Of_Ghyran),
            ToInteger(CommandTrait::Lord_Of_Spites),
            ToInteger(CommandTrait::Warsinger),
            ToInteger(CommandTrait::Wisdom_Of_The_Ancients),
    };

    constexpr std::array<int, 7> g_aspectsOfRenewal = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arcane_Bounty),
            ToInteger(CommandTrait::Mystic_Regrowth),
            ToInteger(CommandTrait::Voice_Of_Warding),
            ToInteger(CommandTrait::Glade_Lore),
            ToInteger(CommandTrait::Spellsinger),
            ToInteger(CommandTrait::Radiant_Spirit),
    };

    constexpr std::array<int, 19> g_artefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Daiths_Reaper),
            ToInteger(Artefact::Greenwood_Gladius),
            ToInteger(Artefact::Autumns_Ire),
            ToInteger(Artefact::Winnowstaff),
            ToInteger(Artefact::Ancient_Barkblade),
            ToInteger(Artefact::The_Darkest_Bough),
            ToInteger(Artefact::The_Oaken_Armour),
            ToInteger(Artefact::Briar_Sheath),
            ToInteger(Artefact::Glamourweave),
            ToInteger(Artefact::Lashvines),
            ToInteger(Artefact::Silken_Snares),
            ToInteger(Artefact::Nightbloom_Garland),
            ToInteger(Artefact::Seed_Of_Rebirth),
            ToInteger(Artefact::Wraithstone),
            ToInteger(Artefact::Everdew_Vial),
            ToInteger(Artefact::Lifewreath),
            ToInteger(Artefact::Crown_Of_Fell_Bowers),
            ToInteger(Artefact::Etherblossom),
    };

    constexpr std::array<int, 7> g_relicsOfNature = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Acorn_Of_The_Ages),
            ToInteger(Artefact::Spiritsong_Stave),
            ToInteger(Artefact::The_Vesperal_Gem),
            ToInteger(Artefact::Luneths_Lamp),
            ToInteger(Artefact::Hagbone_Spite),
            ToInteger(Artefact::Wychwood_Glaive),
    };

    constexpr std::array<int, 7> g_loreOfTheDeepwood = {
            ToInteger(Lore::None),
            ToInteger(Lore::Throne_Of_Vines),
            ToInteger(Lore::Regrowth),
            ToInteger(Lore::The_Dwellers_Below),
            ToInteger(Lore::Deadly_Harvest),
            ToInteger(Lore::Verdurous_Harmony),
            ToInteger(Lore::Treesong),
    };

} // namespace Sylvaneth
