/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMCASTETERNALS_H
#define STORMCASTETERNALS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace StormcastEternals {

    //
    // Command Traits
    //
    enum class CommandTrait : int {
        None,

        // AspectsOfAzyr
        Shielded_By_Faith,
        Consummate_Commander,
        Cunning_Strategist,
        Zealous_Crusader,
        Staunch_Defender,
        Champion_Of_The_Realms,

        // Stormhosts
        We_Cannot_Fail, // Hammers of Sigmar
        Martyrs_Strength, // Hallowed Knights
        Single_Minded_Fury, // Celestial Vindicators
        Deathly_Aura, // Anvils
        Divine_Executioner, // Knights Excelsior
        Portents_And_Omens, // Celestial Warbringers
        Bonds_Of_Noble_Duty, // Tempest Lords
        Dauntless_Hunters // Astral Templars
    };

    //
    // Artefacts
    //
    enum class Artefact : int {
        None,

        // StormForgedWeapons
        Strife_Ender,
        Blade_Of_Heroes,
        Hammer_Of_Might,
        Fang_Of_Dracothian,
        Obsidian_Blade,
        Gift_Of_The_Six_Smiths,

        // HeavenWroughtArmour
        Armour_Of_Destiny,
        Armour_Of_Silvered_Sigmarite,
        Drakescale_Armour,
        Mirrorshield,
        Spellshield,
        Featherfoe_Torc,

        // ArtefactsOfTheTempests
        Talisman_Of_Endurance,
        Obsidian_Amulet,
        Luckstone,
        Seed_Of_Rebirth,
        Sigmarite_Pendant,
        Quicksilver_Draught,

        // TreasuredStandards
        Hurricane_Standard,
        Lichebone_Standard,
        Pennant_Of_Sigmaron,

        // MysticLights
        Shriving_Light,
        Fury_Brand,
        Lantern_Of_The_Tempest,

        // CelestialStaves
        Staff_Of_Focus,
        Mindlock_Staff,
        Staff_Of_Azyr,

        // ScrollsOfPower
        Scroll_Of_Unravelling,
        Scroll_Of_Condemnation,
        Storm_Scroll,

        // Stormhost
        God_Forged_Blade,   // Hammers of Sigmar
        Parchment_Of_Purity, // Hallowed Knights
        Stormrage_Blade, // Celestial Vindicators
        Soulthief, // Anvils
        Chains_Of_Celestial_Lightning, // Knights Excelsior
        Hammers_Of_Augury, // Celestial Warbringers
        Patricians_Helm, // Tempest Lords
        Godbeast_Plate // Astral Templars
    };

    enum class Stormhost : int {
        None = 0,

        Hammers_Of_Sigmar,
        Hallowed_Knights,
        Celestial_Vindicators,
        Anvils_Of_The_Heldenhammer,
        Knights_Excelsior,
        Celestial_Warbringers,
        Tempest_Lords,
        Astral_Templars,
    };

    class StormcastEternal : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        StormcastEternal() = default;

        ~StormcastEternal() override = default;

        void setStormhost(Stormhost host);
        void setArtefact(Artefact artefact);
        void setCommandTrait(CommandTrait commandTrait);

    protected:
        StormcastEternal(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int braveryModifier() const override;

        Rerolls battleshockRerolls() const override;

        void onStartHero(PlayerId player) override;

    protected:

        Stormhost m_stormHost = Stormhost::None;
        Artefact m_artefact = Artefact::None;
        CommandTrait m_commandTrait = CommandTrait::None;
    };

    void Init();

    bool DoSpiritFlasks(Unit *owner);

//
// Abilities                    Implemented
// -------------------------------------------
// Scions of the Storm              TODO
// Shock and Awe                    TODO
// Hammers of Sigmar
//   First to be Forged             Yes
//   Soul of the Stormhost          TODO
//   We Cannon Fail                 TODO
// Hallowed Knights
//   Only the Faithful              TODO
//   Holy Crusaders                 TODO
//   Martyr's Strength              TODO
// Celestial Vindicators
//   Driven by Vengeance            Yes
//   Righteous Hatred               TODO
//   Single-minded Fury             TODO
// Anvils of the Heldenhammer
//   No True Death                  Yes
//   Heroes of Another Age          TODO
//   Deathly Aura                   TODO
// Knights Excelsior
//   Storm of Annihilation          TODO
//   No Mercy                       TODO
//   Divine Executioner             TODO
// Celestial Warbringers
//   Fearless Foresight             TODO
//   Astral Conjunction             TODO
//   Portents and Omens             TODO
// Tempest Lords
//   Grand Strategists              Yes
//   Rousing Oratory                TODO
//   Bonds of Noble Duty            TODO
// Astral Templars
//   Beast Stalkers                 Yes
//   Cut off the Head               TODO
//   Dauntless Hunters              TODO
//

} // namespace StormcastEternals

#endif //STORMCASTETERNALS_H