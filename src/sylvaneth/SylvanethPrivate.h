/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SYLVANETHPRIVATE_H
#define SYLVANETHPRIVATE_H

#include <sylvaneth/SylvanethBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Sylvaneth {

    const std::array<int, 8> g_glade = {
            to_integer(Glade::None),
            to_integer(Glade::Oakenbrow),
            to_integer(Glade::Gnarlroot),
            to_integer(Glade::Heartwood),
            to_integer(Glade::Ironbark),
            to_integer(Glade::Winterleaf),
            to_integer(Glade::Dreadwood),
            to_integer(Glade::Harvestboon)
    };

    constexpr std::array<int, 7> g_aspectsOfWar = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Dread_Harvester),
            to_integer(CommandTrait::Gnarled_Warrior),
            to_integer(CommandTrait::Gift_of_Ghyran),
            to_integer(CommandTrait::Lord_of_Spites),
            to_integer(CommandTrait::Warsinger),
            to_integer(CommandTrait::Wisdom_of_the_Ancients),
    };

    constexpr std::array<int, 7> g_aspectsOfRenewal = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arcane_Bounty),
            to_integer(CommandTrait::Mystic_Regrowth),
            to_integer(CommandTrait::Voice_of_Warding),
            to_integer(CommandTrait::Glade_Lore),
            to_integer(CommandTrait::Spellsinger),
            to_integer(CommandTrait::Radiant_Spirit),
    };

    constexpr std::array<int, 19> g_artefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Daiths_Reaper),
            to_integer(Artefact::Greenwood_Gladius),
            to_integer(Artefact::Autumns_Ire),
            to_integer(Artefact::Winnowstaff),
            to_integer(Artefact::Ancient_Barkblade),
            to_integer(Artefact::The_Darkest_Bough),
            to_integer(Artefact::The_Oaken_Armour),
            to_integer(Artefact::Briar_Sheath),
            to_integer(Artefact::Glamourweave),
            to_integer(Artefact::Lashvines),
            to_integer(Artefact::Silken_Snares),
            to_integer(Artefact::Nightbloom_Garland),
            to_integer(Artefact::Seed_of_Rebirth),
            to_integer(Artefact::Wraithstone),
            to_integer(Artefact::Everdew_Vial),
            to_integer(Artefact::Lifewreath),
            to_integer(Artefact::Crown_of_Fell_Bowers),
            to_integer(Artefact::Etherblossom),
    };

    constexpr std::array<int, 7> g_relicsOfNature = {
            to_integer(Artefact::None),
            to_integer(Artefact::Acorn_of_the_Ages),
            to_integer(Artefact::Spiritsong_Stave),
            to_integer(Artefact::The_Vesperal_Gem),
            to_integer(Artefact::Luneths_Lamp),
            to_integer(Artefact::Hagbone_Spite),
            to_integer(Artefact::Wychwood_Glaive),
    };

    constexpr std::array<int, 7> g_loreOfTheDeepwood = {
        to_integer(Lore::None),
        to_integer(Lore::Throne_of_Vines),
        to_integer(Lore::Regrowth),
        to_integer(Lore::The_Dwellers_Below),
        to_integer(Lore::Deadly_Harvest),
        to_integer(Lore::Verdurous_Harmony),
        to_integer(Lore::Treesong),
    };

} // namespace Sylvaneth

#endif // SYLVANETHPRIVATE_H