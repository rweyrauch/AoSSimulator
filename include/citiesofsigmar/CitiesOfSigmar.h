/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CITIESOFSIGMAR_H
#define CITIESOFSIGMAR_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    enum class City : int {
        Hammerhal,
        Living_City,
        Greywater_Fastness,
        Phoenicium,
        Anvilgard,
        Hallowheart,
        Tempests_Eye
    };

    enum class CommandTrait : int {
        None,

        // Hammerhal
        Acadamae_Prodigy,
        Blood_of_the_Twelve,
        Aggressive_General,

        // Living City
        Ironoak_Artisan,
        Forest_Strider,
        Druid_of_the_Everspring,

        // Greywater Fastness
        Seat_on_the_Council,
        Drillmaster,
        Ghoul_Mere_Ranger,

        // Phoenicium
        Seeder_of_Vengeance,
        One_with_Fire_and_Ice,
        Aura_of_Serenity,

        // Anvilgard
        Blackfang_Crimelord,
        Slayer_of_Monsters,
        Secretive_Warlock,

        // Hallowheart
        Veteran_of_the_Blazing_Crusade,
        Warden_of_the_Flame,
        Famed_Spell_Hunter,

        // Tempests Eye
        Aetherguard_Captain,
        Hawk_Eyed,
        Swift_as_the_Wind,

    };

    enum class Artefact : int {
        None,

        // Hammerhal
        Armour_of_Mallus,
        Saints_Blade,
        The_Twinstone,

        // Living City
        Spear_of_the_Hunt,
        Deepmire_Cloak,
        Wardroth_Horn,

        // Greywater Fastness
        Steam_Piston_Plate_Mail,
        Runic_Munitions,
        Mastro_Vivettis_Magnificent_Macroscrope,

        // Phoenicium
        Amber_Armour,
        Phoenix_Pinion,
        Phoenix_Pyre_Ashes,

        // Anvilgard
        Acidic_Blood,
        Jutting_Bones,
        Fell_Gaze,

        // Hallowheart
        Agloraxi_Prism,
        Pauldrons_of_Living_Flame,
        Whitefire_Tome,

        // Tempests Eye
        Patricians_Helm,
        Seerstone_Amulet,
        Zephyrite_Banner,

    };

    enum class Lore : int {
        None,

        // Hammerhal
        Wings_of_Fire,
        Cindercloud,
        Twin_Tailed_Comet,

        // Living City
        Lifesurge,
        Cage_of_Thorns,
        Ironoak_Skin,

        // Greywater Fastness
        Descending_Ash_Cloud,
        Eroding_Blast,
        Choking_Fumes,

        // Phoenicium
        Amber_Tide,
        Phoenix_Cry,
        Golden_Mist,

        // Anvilgard
        Sap_Strength,
        Shadow_Daggers,
        Vitriolic_Spray,

        // Hallowheart
        Roaming_Wildfire,
        Sear_Wounds,
        Elemental_Cyclone,
        Warding_Brand,
        Crystal_Aegis,
        Ignite_Weapons,

        // Tempests Eye
        Aura_of_Glory,
        Strike_of_Eagles,
        Celestial_Visions,
    };

    class CitizenOfSigmar : public Unit {
    public:
        CitizenOfSigmar() = default;

        ~CitizenOfSigmar() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setCity(City city);

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        CitizenOfSigmar(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int runModifier() const override;
        int moveModifier() const override;
        int toSaveModifier(const Weapon *weapon) const override;
        void onStartHero(PlayerId player) override;

    protected:

        City m_city = City::Hammerhal;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Amplified Sorceries              TODO
// Loyal Shields                    TODO
// Wise Council                     TODO
// Hammerhal
//    Banners Held High             TODO
//    The Magister of Hammerhal     TODO
//    The Pride of Hammerhal        TODO
//    Righteous Purpose             TODO
// Living City
//    Hunters of the Hidden Paths   TODO
//    Attuned to Nature             Yes
//    Strike then Melt Away         TODO
// Greywater Fastness
//    Rune Lore                     TODO
//    Home of the Great Ironweld Guilds     TODO
//    Salvo Fire                    TODO
// The Phoenicium
//    Vengeful Revenants            TODO
//    Blood of the Ur-Phoenix       Yes
//    Living Idols                  TODO
// Anvilgard
//    Black Market Bounty           TODO
//    Dabblings in Sorcery          TODO
//    Hidden Agents                 TODO
//    Drakeblood Curses             TODO
//    Make an Example of the Weak   TODO
// Hallowheart
//    Eldritch Attunement           TODO
//    Mages of the Whitefire Court  TODO
//    Arcane Channelling            TODO
// Tempest's Eye
//    Alert and Forewarned          Yes
//    Outriders of the Realms       Yes
//    Standing Contracts            TODO
//    Rapid Redeploy                TODO
//

    void Init();

} // namespace CitiesOfSigmar

#endif // CITIESOFSIGMAR_H