/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

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
        Tempests_Eye,
        Misthåvn,
        Har_Kuron,
        Settlers_Gain
    };

    enum class CommandTrait : int {
        None,

        // Hammerhal
        Acadamae_Prodigy,
        Blood_Of_The_Twelve,
        Aggressive_General,

        // Living City
        Ironoak_Artisan,
        Forest_Strider,             // TODO
        Druid_Of_The_Everspring,    // TODO

        // Greywater Fastness
        Seat_On_The_Council,
        Drillmaster,
        Ghoul_Mere_Ranger,

        // Phoenicium
        Seeder_Of_Vengeance,        // TODO
        One_With_Fire_And_Ice,      // TODO
        Aura_Of_Serenity,

        // Anvilgard
        Blackfang_Crimelord,        // TODO
        Slayer_Of_Monsters,
        Secretive_Warlock,          // TODO

        // Hallowheart
        Veteran_Of_The_Blazing_Crusade,
        Warden_Of_The_Flame,
        Famed_Spell_Hunter,         // TODO

        // Tempests Eye
        Aetherguard_Captain,
        Hawk_Eyed,
        Swift_As_The_Wind,          // TODO

        // Misthåvn
        Shadowlord,                 // TODO
        Wily_Foe,
        Shade_Warlock,              // TODO

        // Har_Kuron
        Bathed_In_Blood,
        Murderous_Zeal,             // TODO
        Dark_Adept,                 // TODO

        // Settler's Gain
        Personal_Levitation,
        Strategic_Mastermind,
        Raging_Outburst,
    };

    enum class Artefact : int {
        None,

        // Hammerhal
        Armour_Of_Mallus,
        Saints_Blade,
        The_Twinstone,

        // Living City
        Spear_Of_The_Hunt,
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
        Pauldrons_Of_Living_Flame,
        Whitefire_Tome,

        // Tempests Eye
        Patricians_Helm,
        Seerstone_Amulet,
        Zephyrite_Banner,

        // Misthåvn
        Gloom_Bell,
        Stangler_Kelp_Noose,
        Shadowsilk_Armour,

        // Har Kuron
        Lifetaker,
        Traitors_Banner,
        Nullstone_Vizard,

        // Settler's Gain
        Heart_Stone,
        Talisman_Of_Dispellation,
        Silver_Plated_Wand,
        Blade_Of_Leaping_Bronze,
        Amulet_Of_Haste,
        Stone_Spirit_Armour,
    };

    enum class Lore : int {
        None,

        // Hammerhal
        Wings_Of_Fire,
        Cindercloud,
        Twin_Tailed_Comet,

        // Living City
        Lifesurge,
        Cage_Of_Thorns,
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
        Aura_Of_Glory,
        Strike_Of_Eagles,
        Celestial_Visions,

        // Har Kuron
        //Sap_Strength,
        The_Withering,
        //Vitriolic_Spray,
        Steed_Of_Shadows,
        Pit_Of_Shades,
        //Shadow_Daggers,

        // Settler's Gain
        Drain_Magic,
        Shield_Of_Light,
        Illuminate,
    };

    enum class Narcotic : int {
        None,
        Synesthalcum,       // TODO
        Witch_Mist,         // TODO
        Skiffers_Salve,     // TODO
        Float,              // TODO
        Sawfang_Dust,       // TODO
        Glatch_Ink          // TODO
    };

    class CitizenOfSigmar : public Unit {
    public:
        CitizenOfSigmar() = delete;

        ~CitizenOfSigmar() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setCity(City city);

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
        void setNarcotic(Narcotic narcotic);

    protected:
        CitizenOfSigmar(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int runModifier() const override;

        int moveModifier() const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        void onStartHero(PlayerId player) override;

        bool battleshockRequired() const override;

        void onBeginRound(int battleRound) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onStartMovement(PlayerId player) override;

        int chargeModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int woundModifier() const override;

    protected:

        City m_city = City::Hammerhal;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
        Narcotic m_narcotic = Narcotic::None;

        int m_bannersHeldHigh = 0;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Amplified Sorceries              TODO
// Loyal Shields                    TODO
// Wise Council                     TODO
// Hammerhal
//    Banners Held High             Yes
//    The Magister of Hammerhal     Yes
//    The Pride of Hammerhal        Yes
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
// Misthåvn
//    Underhanded Tactics           TODO
//    Narcotics                     TODO
//    Shadowstrike                  TODO
// Har_Kuron
//    Incitement to Murder          TODO
//    Make an Example of the Weak   TODO
// Settler's Gain
//    Lumineth Tutors               TODO
//    Warriors of the High Districts    TODO
//    Aelven Training               TODO
//

    void Init();

} // namespace CitiesOfSigmar
