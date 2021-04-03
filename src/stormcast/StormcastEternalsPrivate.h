/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <stormcast/PrayersOfTheStormhosts.h>
#include <stormcast/LoreOfTheStorm.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace StormcastEternals {

    constexpr std::array<int, 9> g_stormhost = {
            ToInteger(Stormhost::None),
            ToInteger(Stormhost::Hammers_Of_Sigmar),
            ToInteger(Stormhost::Hallowed_Knights),
            ToInteger(Stormhost::Celestial_Vindicators),
            ToInteger(Stormhost::Anvils_Of_The_Heldenhammer),
            ToInteger(Stormhost::Knights_Excelsior),
            ToInteger(Stormhost::Celestial_Warbringers),
            ToInteger(Stormhost::Tempest_Lords),
            ToInteger(Stormhost::Astral_Templars),
    };

    constexpr std::array<int, 7> g_commandTrait{
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Shielded_By_Faith),
            ToInteger(CommandTrait::Consummate_Commander),
            ToInteger(CommandTrait::Cunning_Strategist),
            ToInteger(CommandTrait::Zealous_Crusader),
            ToInteger(CommandTrait::Staunch_Defender),
            ToInteger(CommandTrait::Champion_Of_The_Realms),
    };

    constexpr std::array<int, 19> g_artefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Strife_Ender),
            ToInteger(Artefact::Blade_Of_Heroes),
            ToInteger(Artefact::Hammer_Of_Might),
            ToInteger(Artefact::Fang_Of_Dracothian),
            ToInteger(Artefact::Obsidian_Blade),
            ToInteger(Artefact::Gift_Of_The_Six_Smiths),
            ToInteger(Artefact::Armour_Of_Destiny),
            ToInteger(Artefact::Armour_Of_Silvered_Sigmarite),
            ToInteger(Artefact::Drakescale_Armour),
            ToInteger(Artefact::Mirrorshield),
            ToInteger(Artefact::Spellshield),
            ToInteger(Artefact::Featherfoe_Torc),
            ToInteger(Artefact::Talisman_Of_Endurance),
            ToInteger(Artefact::Obsidian_Amulet),
            ToInteger(Artefact::Luckstone),
            ToInteger(Artefact::Seed_Of_Rebirth),
            ToInteger(Artefact::Sigmarite_Pendant),
            ToInteger(Artefact::Quicksilver_Draught)
    };

    constexpr std::array<int, 22> g_artefactsTotems = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Strife_Ender),
            ToInteger(Artefact::Blade_Of_Heroes),
            ToInteger(Artefact::Hammer_Of_Might),
            ToInteger(Artefact::Fang_Of_Dracothian),
            ToInteger(Artefact::Obsidian_Blade),
            ToInteger(Artefact::Gift_Of_The_Six_Smiths),
            ToInteger(Artefact::Armour_Of_Destiny),
            ToInteger(Artefact::Armour_Of_Silvered_Sigmarite),
            ToInteger(Artefact::Drakescale_Armour),
            ToInteger(Artefact::Mirrorshield),
            ToInteger(Artefact::Spellshield),
            ToInteger(Artefact::Featherfoe_Torc),
            ToInteger(Artefact::Talisman_Of_Endurance),
            ToInteger(Artefact::Obsidian_Amulet),
            ToInteger(Artefact::Luckstone),
            ToInteger(Artefact::Seed_Of_Rebirth),
            ToInteger(Artefact::Sigmarite_Pendant),
            ToInteger(Artefact::Quicksilver_Draught),
            ToInteger(Artefact::Hurricane_Standard),
            ToInteger(Artefact::Lichebone_Standard),
            ToInteger(Artefact::Pennant_Of_Sigmaron)
    };

    constexpr std::array<int, 22> g_artefactsLights = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Strife_Ender),
            ToInteger(Artefact::Blade_Of_Heroes),
            ToInteger(Artefact::Hammer_Of_Might),
            ToInteger(Artefact::Fang_Of_Dracothian),
            ToInteger(Artefact::Obsidian_Blade),
            ToInteger(Artefact::Gift_Of_The_Six_Smiths),
            ToInteger(Artefact::Armour_Of_Destiny),
            ToInteger(Artefact::Armour_Of_Silvered_Sigmarite),
            ToInteger(Artefact::Drakescale_Armour),
            ToInteger(Artefact::Mirrorshield),
            ToInteger(Artefact::Spellshield),
            ToInteger(Artefact::Featherfoe_Torc),
            ToInteger(Artefact::Talisman_Of_Endurance),
            ToInteger(Artefact::Obsidian_Amulet),
            ToInteger(Artefact::Luckstone),
            ToInteger(Artefact::Seed_Of_Rebirth),
            ToInteger(Artefact::Sigmarite_Pendant),
            ToInteger(Artefact::Quicksilver_Draught),
            ToInteger(Artefact::Shriving_Light),
            ToInteger(Artefact::Fury_Brand),
            ToInteger(Artefact::Lantern_Of_The_Tempest)
    };

    constexpr std::array<int, 25> g_celestialMagic = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Strife_Ender),
            ToInteger(Artefact::Blade_Of_Heroes),
            ToInteger(Artefact::Hammer_Of_Might),
            ToInteger(Artefact::Fang_Of_Dracothian),
            ToInteger(Artefact::Obsidian_Blade),
            ToInteger(Artefact::Gift_Of_The_Six_Smiths),
            ToInteger(Artefact::Armour_Of_Destiny),
            ToInteger(Artefact::Armour_Of_Silvered_Sigmarite),
            ToInteger(Artefact::Drakescale_Armour),
            ToInteger(Artefact::Mirrorshield),
            ToInteger(Artefact::Spellshield),
            ToInteger(Artefact::Featherfoe_Torc),
            ToInteger(Artefact::Talisman_Of_Endurance),
            ToInteger(Artefact::Obsidian_Amulet),
            ToInteger(Artefact::Luckstone),
            ToInteger(Artefact::Seed_Of_Rebirth),
            ToInteger(Artefact::Sigmarite_Pendant),
            ToInteger(Artefact::Quicksilver_Draught),
            ToInteger(Artefact::Staff_Of_Focus),
            ToInteger(Artefact::Mindlock_Staff),
            ToInteger(Artefact::Staff_Of_Azyr),
            ToInteger(Artefact::Scroll_Of_Unravelling),
            ToInteger(Artefact::Scroll_Of_Condemnation),
            ToInteger(Artefact::Storm_Scroll)
    };

    constexpr std::array<int, 3> g_loreOfInvigoration = {
            ToInteger(Lore::Terrifying_Aspect),
            ToInteger(Lore::Celestial_Blades),
            ToInteger(Lore::Speed_Of_Lightning)
    };

    constexpr std::array<int, 9> g_lore = {
            ToInteger(Lore::Lightning_Blast),
            ToInteger(Lore::Starfall),
            ToInteger(Lore::Thundershock),
            ToInteger(Lore::Azyrite_Halo),
            ToInteger(Lore::Chain_Lightning),
            ToInteger(Lore::Stormcaller),
            ToInteger(Lore::Terrifying_Aspect),
            ToInteger(Lore::Celestial_Blades),
            ToInteger(Lore::Speed_Of_Lightning)
    };


    constexpr std::array<int, 6> g_prayersOfTheStormhost = {
            ToInteger(PrayersOfTheStormhost::Divine_Light),
            ToInteger(PrayersOfTheStormhost::Bless_Weapons),
            ToInteger(PrayersOfTheStormhost::Bolster_Faith),
            ToInteger(PrayersOfTheStormhost::Abjuration),
            ToInteger(PrayersOfTheStormhost::God_Kings_Aspect),
            ToInteger(PrayersOfTheStormhost::Translocation),
    };

    constexpr std::array<int, 9> g_stormhostCommand = {
            ToInteger(Command::None),
            ToInteger(Command::Soul_Of_The_Stormhost),
            ToInteger(Command::Holy_Crusaders),
            ToInteger(Command::Righteous_Hatred),
            ToInteger(Command::Heroes_Of_Another_Age),
            ToInteger(Command::No_Mercy),
            ToInteger(Command::Astral_Conjunction),
            ToInteger(Command::Rousing_Oratory),
            ToInteger(Command::Cut_Off_The_Head)
    };

    constexpr std::array<int, 7> g_dracothMountTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Lithe_Limbed),
            ToInteger(MountTrait::Keen_Clawed),
            ToInteger(MountTrait::Savage_Loyalty),
            ToInteger(MountTrait::Drake_Kin),
            ToInteger(MountTrait::Thunder_Caller),
            ToInteger(MountTrait::Pack_Leader)
    };

    constexpr std::array<int, 7> g_stardrakeMountTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Lithe_Limbed),
            ToInteger(MountTrait::Keen_Clawed),
            ToInteger(MountTrait::Savage_Loyalty),
            ToInteger(MountTrait::Storm_Winged),
            ToInteger(MountTrait::Thunderlord),
            ToInteger(MountTrait::Star_Branded)
    };

    constexpr std::array<int, 7> g_gryphChargerMountTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Lithe_Limbed),
            ToInteger(MountTrait::Keen_Clawed),
            ToInteger(MountTrait::Savage_Loyalty),
            ToInteger(MountTrait::Wind_Runner),
            ToInteger(MountTrait::Aethereal_Stalker),
            ToInteger(MountTrait::Indefatigable)
    };

    constexpr std::array<int, 7> g_tauralonMountTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Lithe_Limbed),
            ToInteger(MountTrait::Keen_Clawed),
            ToInteger(MountTrait::Savage_Loyalty),
            ToInteger(MountTrait::Swiftwing),
            ToInteger(MountTrait::Lashing_Tail),
            ToInteger(MountTrait::Steel_Pinions)
    };

    constexpr std::array<int, 7> g_dracolineMountTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Lithe_Limbed),
            ToInteger(MountTrait::Keen_Clawed),
            ToInteger(MountTrait::Savage_Loyalty),
            ToInteger(MountTrait::Bounding_Leap),
            ToInteger(MountTrait::Pride_Leader),
            ToInteger(MountTrait::Ear_Bursting_Roar)
    };

    CommandAbility *CreateCommandAbility(Command which, Unit *source);

} // namespace StormcastEternals
