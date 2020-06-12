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
        Champion_of_the_Realms,

        // Stormhosts
        We_Cannot_Fail, // Hammers of Sigmar
        Martyrs_Strength, // Hallowed Knights
        Single_Minded_Fury, // Celestial Vindicators
        Deathly_Aura, // Anvils
        Divine_Executioner, // Knights Excelsior
        Portents_and_Omens, // Celestial Warbringers
        Bonds_of_Noble_Duty, // Tempest Lords
        Dauntless_Hunters // Astral Templars
    };

    //
    // Artefacts
    //
    enum class Artefact : int {
        None,

        // StormForgedWeapons
        Strife_Ender,
        Blade_of_Heroes,
        Hammer_of_Might,
        Fang_of_Dracothian,
        Obsidian_Blade,
        Gift_of_the_Six_Smiths,

        // HeavenWroughtArmour
        Armour_of_Destiny,
        Armour_of_Silvered_Sigmarite,
        Drakescale_Armour,
        Mirrorshield,
        Spellshield,
        Featherfoe_Torc,

        // ArtefactsOfTheTempests
        Talisman_of_Endurance,
        Obsidian_Amulet,
        Luckstone,
        Seed_of_Rebirth,
        Sigmarite_Pendant,
        Quicksilver_Draught,

        // TreasuredStandards
        Hurricane_Standard,
        Lichebone_Standard,
        Pennant_of_Sigmaron,

        // MysticLights
        Shriving_Light,
        Fury_Brand,
        Lantern_of_the_Tempest,

        // CelestialStaves
        Staff_of_Focus,
        Mindlock_Staff,
        Staff_of_Azyr,

        // ScrollsOfPower
        Scroll_of_Unravelling,
        Scroll_of_Condemnation,
        Storm_Scroll,

        // Stormhost
        God_Forged_Blade,   // Hammers of Sigmar
        Parchment_of_Purity, // Hallowed Knights
        Stormrage_Blade, // Celestial Vindicators
        Soulthief, // Anvils
        Chains_of_Celestial_Lightning, // Knights Excelsior
        Hammers_of_Augury, // Celestial Warbringers
        Patricians_Helm, // Tempest Lords
        Godbeast_Plate // Astral Templars
    };

    enum class Stormhost : int {
        None = 0,

        Hammers_of_Sigmar,
        Hallowed_Knights,
        Celestial_Vindicators,
        Anvils_of_the_Heldenhammer,
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

} // namespace StormcastEternals

#endif //STORMCASTETERNALS_H