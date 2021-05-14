/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Sylvaneth {

    enum class Glade : int {
        None = 0,
        Oakenbrow,
        Gnarlroot,
        Heartwood,
        Ironbark,
        Winterleaf,
        Dreadwood,
        Harvestboon
    };

    enum class CommandTrait : int {
        None,

        // Aspects of War
        Dread_Harvester,
        Gnarled_Warrior,
        Gift_Of_Ghyran,
        Lord_Of_Spites,
        Warsinger,
        Wisdom_Of_The_Ancients,

        // Aspects of Renewal
        Arcane_Bounty,
        Mystic_Regrowth,
        Voice_Of_Warding,
        Glade_Lore,
        Spellsinger,            // TODO
        Radiant_Spirit,

        // Glade specific
        Regal_Old_Growth,       // Oakenbrow
        Nurtured_By_Magic,      // Gnarlroot
        Legacy_Of_Valour,       // Heartwood
        Mere_Rainfall,          // Ironbark
        My_Heart_Is_Ice,        // Winterleaf
        Paragon_Of_Terror,      // Dreadwood
        Seek_New_Fruit,         // Harvestboon
    };

    enum class Artefact : int {
        None,

        // Weapons of the Glade
        Daiths_Reaper,
        Greenwood_Gladius,
        Autumns_Ire,
        Winnowstaff,
        Ancient_Barkblade,
        The_Darkest_Bough,

        // Boons of the Everqueen
        The_Oaken_Armour,
        Briar_Sheath,
        Glamourweave,
        Lashvines,
        Silken_Snares,
        Nightbloom_Garland,

        // Verdent Treasures
        Seed_Of_Rebirth,
        Wraithstone,
        Everdew_Vial,
        Lifewreath,
        Crown_Of_Fell_Bowers,
        Etherblossom,

        // Relics of Nature
        Acorn_Of_The_Ages,
        Spiritsong_Stave,
        The_Vesperal_Gem,
        Luneths_Lamp,
        Hagbone_Spite,
        Wychwood_Glaive,

        // Glade specific
        Dawnflask, // Oakenbrow
        Chalice_Of_Nectar, // Gnarlroot
        Horn_Of_The_Consort, // Heartwood
        Ironbark_Talisman, // Ironbark
        Frozen_Kernel, // Winterleaf
        Jewel_Of_Withering, // Dreadwood
        The_Silent_Sickle, // Harvestboon
    };

    enum class Lore : int {
        None,

        // Lore of the Deepwood
        Throne_Of_Vines,
        Regrowth,
        The_Dwellers_Below,
        Deadly_Harvest,
        Verdurous_Harmony,
        Treesong,
    };

    class SylvanethBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SylvanethBase() = delete;

        ~SylvanethBase() override = default;

        void setGlade(Glade glade);

        void setArtefact(Artefact artefact);

        void setCommandTrait(CommandTrait commandTrait);

    protected:
        SylvanethBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls battleshockRerolls() const override;

        int braveryModifier() const override;

        int woundModifier() const override;

        void onCastSpell(const Spell *spell, const Unit *target) override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        void onStartHero(PlayerId player) override;

        int chargeModifier() const override;

        int castingModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    protected:

        Glade m_glade = Glade::None;
        Artefact m_artefact = Artefact::None;
        CommandTrait m_commandTrait = CommandTrait::None;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Forest Spirits                   TODO
// Navigate Realmroots              TODO
// Places of Power                  TODO
// Oakenbrow
//   Our Roots Run Deep             Yes
//   Yield to None                  TODO
//   Regal Old-growth               Yes
// Gnarlroot
//   Shield the Arcane              Yes
//   The Earth Defends              TODO
//   Nurtured by Magic              Yes
// Heartwood
//   Courage For Kurnoth            Yes
//   Lord of the Hunt               TODO
//   Legacy of Valour               Yes
// Ironbark
//   Stubborn and Taciturn          Yes
//   Stand Firm                     TODO
//   Mere Rainfall                  Yes
// Winterleaf
//   Winter's Bite                  Yes
//   Branch Blizzard                TODO
//   My Heart is Ice                TODO
// Dreadwood
//   Malicious Tormentors           Yes
//   Sinister Ambush                TODO
//   Paragon of Terror              TODO
// Harvestboon
//   Vibrant Surge                  Yes
//   Fertile Ground                 TODO
//   Seek New Fruit                 TODO
//

} // namespace Sylvaneth
