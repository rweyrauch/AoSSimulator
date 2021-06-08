/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace CitiesOfSigmar {

    constexpr std::array<int, 9> g_realm = {
            ToInteger(Realm::None),
            ToInteger(Realm::Aqshy),
            ToInteger(Realm::Azyr),
            ToInteger(Realm::Chamon),
            ToInteger(Realm::Ghur),
            ToInteger(Realm::Ghyran),
            ToInteger(Realm::Hysh),
            ToInteger(Realm::Shyish),
            ToInteger(Realm::Ulgu)
    };

    constexpr std::array<int, 11> g_city = {
            ToInteger(City::Hammerhal),
            ToInteger(City::Living_City),
            ToInteger(City::Greywater_Fastness),
            ToInteger(City::Phoenicium),
            ToInteger(City::Anvilgard),
            ToInteger(City::Hallowheart),
            ToInteger(City::Tempests_Eye),
            ToInteger(City::Misthåvn),
            ToInteger(City::Har_Kuron),
            ToInteger(City::Settlers_Gain),
            ToInteger(City::Excelsis)
    };

    constexpr std::array<int, 34> g_commandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Acadamae_Prodigy),
            ToInteger(CommandTrait::Blood_Of_The_Twelve),
            ToInteger(CommandTrait::Aggressive_General),
            ToInteger(CommandTrait::Ironoak_Artisan),
            ToInteger(CommandTrait::Forest_Strider),
            ToInteger(CommandTrait::Druid_Of_The_Everspring),
            ToInteger(CommandTrait::Seat_On_The_Council),
            ToInteger(CommandTrait::Drillmaster),
            ToInteger(CommandTrait::Ghoul_Mere_Ranger),
            ToInteger(CommandTrait::Seeder_Of_Vengeance),
            ToInteger(CommandTrait::One_With_Fire_And_Ice),
            ToInteger(CommandTrait::Aura_Of_Serenity),
            ToInteger(CommandTrait::Blackfang_Crimelord),
            ToInteger(CommandTrait::Slayer_Of_Monsters),
            ToInteger(CommandTrait::Secretive_Warlock),
            ToInteger(CommandTrait::Veteran_Of_The_Blazing_Crusade),
            ToInteger(CommandTrait::Warden_Of_The_Flame),
            ToInteger(CommandTrait::Famed_Spell_Hunter),
            ToInteger(CommandTrait::Aetherguard_Captain),
            ToInteger(CommandTrait::Hawk_Eyed),
            ToInteger(CommandTrait::Swift_As_The_Wind),
            ToInteger(CommandTrait::Shadowlord),
            ToInteger(CommandTrait::Wily_Foe),
            ToInteger(CommandTrait::Shade_Warlock),
            ToInteger(CommandTrait::Bathed_In_Blood),
            ToInteger(CommandTrait::Murderous_Zeal),
            ToInteger(CommandTrait::Dark_Adept),
            ToInteger(CommandTrait::Personal_Levitation),
            ToInteger(CommandTrait::Strategic_Mastermind),
            ToInteger(CommandTrait::Raging_Outburst),
            ToInteger(CommandTrait::Cunning_Foe),
            ToInteger(CommandTrait::In_the_Right_Place),
            ToInteger(CommandTrait::Darkest_Secrets),
    };

    constexpr std::array<int, 37> g_artefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Armour_Of_Mallus),
            ToInteger(Artefact::Saints_Blade),
            ToInteger(Artefact::The_Twinstone),
            ToInteger(Artefact::Spear_Of_The_Hunt),
            ToInteger(Artefact::Deepmire_Cloak),
            ToInteger(Artefact::Wardroth_Horn),
            ToInteger(Artefact::Steam_Piston_Plate_Mail),
            ToInteger(Artefact::Runic_Munitions),
            ToInteger(Artefact::Mastro_Vivettis_Magnificent_Macroscrope),
            ToInteger(Artefact::Amber_Armour),
            ToInteger(Artefact::Phoenix_Pinion),
            ToInteger(Artefact::Phoenix_Pyre_Ashes),
            ToInteger(Artefact::Acidic_Blood),
            ToInteger(Artefact::Jutting_Bones),
            ToInteger(Artefact::Fell_Gaze),
            ToInteger(Artefact::Agloraxi_Prism),
            ToInteger(Artefact::Pauldrons_Of_Living_Flame),
            ToInteger(Artefact::Whitefire_Tome),
            ToInteger(Artefact::Patricians_Helm),
            ToInteger(Artefact::Seerstone_Amulet),
            ToInteger(Artefact::Zephyrite_Banner),
            ToInteger(Artefact::Gloom_Bell),
            ToInteger(Artefact::Stangler_Kelp_Noose),
            ToInteger(Artefact::Shadowsilk_Armour),
            ToInteger(Artefact::Lifetaker),
            ToInteger(Artefact::Traitors_Banner),
            ToInteger(Artefact::Nullstone_Vizard),
            ToInteger(Artefact::Heart_Stone),
            ToInteger(Artefact::Talisman_Of_Dispellation),
            ToInteger(Artefact::Silver_Plated_Wand),
            ToInteger(Artefact::Blade_Of_Leaping_Bronze),
            ToInteger(Artefact::Amulet_Of_Haste),
            ToInteger(Artefact::Stone_Spirit_Armour),
            ToInteger(Artefact::Glimmering),
            ToInteger(Artefact::Rockjaws),
            ToInteger(Artefact::Gryph_Feather_Charm)
    };

    constexpr std::array<int, 34> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Wings_Of_Fire),
            ToInteger(Lore::Cindercloud),
            ToInteger(Lore::Twin_Tailed_Comet),
            ToInteger(Lore::Lifesurge),
            ToInteger(Lore::Cage_Of_Thorns),
            ToInteger(Lore::Ironoak_Skin),
            ToInteger(Lore::Descending_Ash_Cloud),
            ToInteger(Lore::Eroding_Blast),
            ToInteger(Lore::Choking_Fumes),
            ToInteger(Lore::Amber_Tide),
            ToInteger(Lore::Phoenix_Cry),
            ToInteger(Lore::Golden_Mist),
            ToInteger(Lore::Sap_Strength),
            ToInteger(Lore::Shadow_Daggers),
            ToInteger(Lore::Vitriolic_Spray),
            ToInteger(Lore::Roaming_Wildfire),
            ToInteger(Lore::Sear_Wounds),
            ToInteger(Lore::Elemental_Cyclone),
            ToInteger(Lore::Warding_Brand),
            ToInteger(Lore::Crystal_Aegis),
            ToInteger(Lore::Ignite_Weapons),
            ToInteger(Lore::Aura_Of_Glory),
            ToInteger(Lore::Strike_Of_Eagles),
            ToInteger(Lore::Celestial_Visions),
            ToInteger(Lore::The_Withering),
            ToInteger(Lore::Steed_Of_Shadows),
            ToInteger(Lore::Pit_Of_Shades),
            ToInteger(Lore::Drain_Magic),
            ToInteger(Lore::Shield_Of_Light),
            ToInteger(Lore::Illuminate),
            ToInteger(Lore::The_Amber_Spear),
            ToInteger(Lore::Flock_of_Doom),
            ToInteger(Lore::Cower)
    };

    constexpr std::array<int, 7> g_narcotic = {
            ToInteger(Narcotic::None),
            ToInteger(Narcotic::Synesthalcum),
            ToInteger(Narcotic::Witch_Mist),
            ToInteger(Narcotic::Skiffers_Salve),
            ToInteger(Narcotic::Float),
            ToInteger(Narcotic::Sawfang_Dust),
            ToInteger(Narcotic::Glatch_Ink)
    };

} // namespace CitiesOfSigmar
