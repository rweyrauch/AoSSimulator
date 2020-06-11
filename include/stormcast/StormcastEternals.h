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
        // AspectsOfAzyr
        Shielded_By_Faith,
        Consummate_Commander,
        Cunning_Strategist,
        Zealous_Crusader,
        Staunch_Defender,
        Champion_of_the_Realms
    };

    //
    // Artefacts
    //
    enum class Artefact : int {
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
        Storm_Scroll
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

    };

    void Init();

    bool DoSpiritFlasks(Unit *owner);

} // namespace StormcastEternals

#endif //STORMCASTETERNALS_H