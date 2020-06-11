/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMCASTETERNALSPRIVATE_H
#define STORMCASTETERNALSPRIVATE_H

#include <stormcast/StormcastEternals.h>
#include <stormcast/PrayersOfTheStormhosts.h>
#include <spells/LoreOfTheStorm.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace StormcastEternals {

    constexpr std::array<int, 9> g_stormhost = {
            to_integer(Stormhost::None),
            to_integer(Stormhost::Hammers_of_Sigmar),
            to_integer(Stormhost::Hallowed_Knights),
            to_integer(Stormhost::Celestial_Vindicators),
            to_integer(Stormhost::Anvils_of_the_Heldenhammer),
            to_integer(Stormhost::Knights_Excelsior),
            to_integer(Stormhost::Celestial_Warbringers),
            to_integer(Stormhost::Tempest_Lords),
            to_integer(Stormhost::Astral_Templars),
    };

    constexpr std::array<int, 6> g_commandTrait {
            to_integer(CommandTrait::Shielded_By_Faith),
            to_integer(CommandTrait::Consummate_Commander),
            to_integer(CommandTrait::Cunning_Strategist),
            to_integer(CommandTrait::Zealous_Crusader),
            to_integer(CommandTrait::Staunch_Defender),
            to_integer(CommandTrait::Champion_of_the_Realms)
    };

    constexpr std::array<int, 6> g_stormForgedWeapons = {
            to_integer(Artefact::Strife_Ender),
            to_integer(Artefact::Blade_of_Heroes),
            to_integer(Artefact::Hammer_of_Might),
            to_integer(Artefact::Fang_of_Dracothian),
            to_integer(Artefact::Obsidian_Blade),
            to_integer(Artefact::Gift_of_the_Six_Smiths)
    };

    constexpr std::array<int, 6> g_heavenWroughtArmour = {
            to_integer(Artefact::Armour_of_Destiny),
            to_integer(Artefact::Armour_of_Silvered_Sigmarite),
            to_integer(Artefact::Drakescale_Armour),
            to_integer(Artefact::Mirrorshield),
            to_integer(Artefact::Spellshield),
            to_integer(Artefact::Featherfoe_Torc)
    };

    constexpr std::array<int, 6> g_artefactsOfTheTempests = {
            to_integer(Artefact::Talisman_of_Endurance),
            to_integer(Artefact::Obsidian_Amulet),
            to_integer(Artefact::Luckstone),
            to_integer(Artefact::Seed_of_Rebirth),
            to_integer(Artefact::Sigmarite_Pendant),
            to_integer(Artefact::Quicksilver_Draught)
    };

    constexpr std::array<int, 3> g_treasuredStandards = {
            to_integer(Artefact::Hurricane_Standard),
            to_integer(Artefact::Lichebone_Standard),
            to_integer(Artefact::Pennant_of_Sigmaron)
    };

    constexpr std::array<int, 3> g_mysticLights = {
            to_integer(Artefact::Shriving_Light),
            to_integer(Artefact::Fury_Brand),
            to_integer(Artefact::Lantern_of_the_Tempest)
    };

    constexpr std::array<int, 3> g_celestialStaves = {
            to_integer(Artefact::Staff_of_Focus),
            to_integer(Artefact::Mindlock_Staff),
            to_integer(Artefact::Staff_of_Azyr)
    };

    constexpr std::array<int, 3> g_scrollsOfPower = {
            to_integer(Artefact::Scroll_of_Unravelling),
            to_integer(Artefact::Scroll_of_Condemnation),
            to_integer(Artefact::Storm_Scroll)
    };


    constexpr std::array<int, 6> g_loreOfTheStorm = {
            to_integer(Lore::Lightning_Blast),
            to_integer(Lore::Starfall),
            to_integer(Lore::Thundershock),
            to_integer(Lore::Azyrite_Halo),
            to_integer(Lore::Chain_Lightning),
            to_integer(Lore::Stormcaller),
    };

    constexpr std::array<int, 3> g_loreOfInvigoration = {
            to_integer(Lore::Terrifying_Aspect),
            to_integer(Lore::Celestial_Blades),
            to_integer(Lore::Speed_of_Lightning)
    };

    constexpr std::array<int, 9> g_lore = {
            to_integer(Lore::Lightning_Blast),
            to_integer(Lore::Starfall),
            to_integer(Lore::Thundershock),
            to_integer(Lore::Azyrite_Halo),
            to_integer(Lore::Chain_Lightning),
            to_integer(Lore::Stormcaller),
            to_integer(Lore::Terrifying_Aspect),
            to_integer(Lore::Celestial_Blades),
            to_integer(Lore::Speed_of_Lightning)
    };


    constexpr std::array<int, 6> g_prayersOfTheStormhost = {
            to_integer(PrayersOfTheStormhost::Divine_Light),
            to_integer(PrayersOfTheStormhost::Bless_Weapons),
            to_integer(PrayersOfTheStormhost::Bolster_Faith),
            to_integer(PrayersOfTheStormhost::Abjuration),
            to_integer(PrayersOfTheStormhost::God_Kings_Aspect),
            to_integer(PrayersOfTheStormhost::Translocation),
    };

} // namespace StormcastEternals

#endif // STORMCASTETERNALSPRIVATE_H