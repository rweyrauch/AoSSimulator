/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CITIESOFSIGMARPRIVATE_H
#define CITIESOFSIGMARPRIVATE_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace CitiesOfSigmar {

    constexpr std::array<int, 7> g_city = {
        to_integer(City::Hammerhal),
        to_integer(City::Living_City),
        to_integer(City::Greywater_Fastness),
        to_integer(City::Phoenicium),
        to_integer(City::Anvilgard),
        to_integer(City::Hallowheart),
        to_integer(City::Tempests_Eye)
    };

    constexpr std::array<int, 22> g_commandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Acadamae_Prodigy),
            to_integer(CommandTrait::Blood_of_the_Twelve),
            to_integer(CommandTrait::Aggressive_General),
            to_integer(CommandTrait::Ironoak_Artisan),
            to_integer(CommandTrait::Forest_Strider),
            to_integer(CommandTrait::Druid_of_the_Everspring),
            to_integer(CommandTrait::Seat_on_the_Council),
            to_integer(CommandTrait::Drillmaster),
            to_integer(CommandTrait::Ghoul_Mere_Ranger),
            to_integer(CommandTrait::Seeder_of_Vengeance),
            to_integer(CommandTrait::One_with_Fire_and_Ice),
            to_integer(CommandTrait::Aura_of_Serenity),
            to_integer(CommandTrait::Blackfang_Crimelord),
            to_integer(CommandTrait::Slayer_of_Monsters),
            to_integer(CommandTrait::Secretive_Warlock),
            to_integer(CommandTrait::Veteran_of_the_Blazing_Crusade),
            to_integer(CommandTrait::Warden_of_the_Flame),
            to_integer(CommandTrait::Famed_Spell_Hunter),
            to_integer(CommandTrait::Aetherguard_Captain),
            to_integer(CommandTrait::Hawk_Eyed),
            to_integer(CommandTrait::Swift_as_the_Wind)
    };

   constexpr std::array<int, 22> g_artefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Armour_of_Mallus),
            to_integer(Artefact::Saints_Blade),
            to_integer(Artefact::The_Twinstone),
            to_integer(Artefact::Spear_of_the_Hunt),
            to_integer(Artefact::Deepmire_Cloak),
            to_integer(Artefact::Wardroth_Horn),
            to_integer(Artefact::Steam_Piston_Plate_Mail),
            to_integer(Artefact::Runic_Munitions),
            to_integer(Artefact::Mastro_Vivettis_Magnificent_Macroscrope),
            to_integer(Artefact::Amber_Armour),
            to_integer(Artefact::Phoenix_Pinion),
            to_integer(Artefact::Phoenix_Pyre_Ashes),
            to_integer(Artefact::Acidic_Blood),
            to_integer(Artefact::Jutting_Bones),
            to_integer(Artefact::Fell_Gaze),
            to_integer(Artefact::Agloraxi_Prism),
            to_integer(Artefact::Pauldrons_of_Living_Flame),
            to_integer(Artefact::Whitefire_Tome),
            to_integer(Artefact::Patricians_Helm),
            to_integer(Artefact::Seerstone_Amulet),
            to_integer(Artefact::Zephyrite_Banner)
    };

    constexpr std::array<int, 25> g_lore = {
            to_integer(Lore::None),
            to_integer(Lore::Wings_of_Fire),
            to_integer(Lore::Cindercloud),
            to_integer(Lore::Twin_Tailed_Comet),
            to_integer(Lore::Lifesurge),
            to_integer(Lore::Cage_of_Thorns),
            to_integer(Lore::Ironoak_Skin),
            to_integer(Lore::Descending_Ash_Cloud),
            to_integer(Lore::Eroding_Blast),
            to_integer(Lore::Choking_Fumes),
            to_integer(Lore::Amber_Tide),
            to_integer(Lore::Phoenix_Cry),
            to_integer(Lore::Golden_Mist),
            to_integer(Lore::Sap_Strength),
            to_integer(Lore::Shadow_Daggers),
            to_integer(Lore::Vitriolic_Spray),
            to_integer(Lore::Roaming_Wildfire),
            to_integer(Lore::Sear_Wounds),
            to_integer(Lore::Elemental_Cyclone),
            to_integer(Lore::Warding_Brand),
            to_integer(Lore::Crystal_Aegis),
            to_integer(Lore::Ignite_Weapons),
            to_integer(Lore::Aura_of_Glory),
            to_integer(Lore::Strike_of_Eagles),
            to_integer(Lore::Celestial_Visions)
    };

} // namespace CitiesOfSigmar

#endif // CITIESOFSIGMARPRIVATE_H