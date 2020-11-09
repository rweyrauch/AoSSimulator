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
            ToInteger(City::Hammerhal),
            ToInteger(City::Living_City),
            ToInteger(City::Greywater_Fastness),
            ToInteger(City::Phoenicium),
            ToInteger(City::Anvilgard),
            ToInteger(City::Hallowheart),
            ToInteger(City::Tempests_Eye)
    };

    constexpr std::array<int, 22> g_commandTraits = {
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
            ToInteger(CommandTrait::Swift_As_The_Wind)
    };

   constexpr std::array<int, 22> g_artefacts = {
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
           ToInteger(Artefact::Zephyrite_Banner)
    };

    constexpr std::array<int, 25> g_lore = {
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
            ToInteger(Lore::Celestial_Visions)
    };

} // namespace CitiesOfSigmar

#endif // CITIESOFSIGMARPRIVATE_H