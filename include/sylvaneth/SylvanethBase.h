/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SYLVANETHBASE_H
#define SYLVANETHBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Sylvaneth {

    enum class Glade: int {
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
        Gift_of_Ghyran,
        Lord_of_Spites,
        Warsinger,
        Wisdom_of_the_Ancients,

        // Aspects of Renewal
        Arcane_Bounty,
        Mystic_Regrowth,
        Voice_of_Warding,
        Glade_Lore,
        Spellsinger,
        Radiant_Spirit,

        // Glade specific
        Regal_Old_Growth, // Oakenbrow,
        Nurtured_by_Magic, // Gnarlroot
        Legacy_of_Valour, // Heartwood
        Mere_Rainfall, // Ironbark
        My_Heart_is_Ice, // Winterleaf
        Paragon_of_Terror, // Dreadwood
        Seek_New_Fruit, // Harvestboon
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
        Seed_of_Rebirth,
        Wraithstone,
        Everdew_Vial,
        Lifewreath,
        Crown_of_Fell_Bowers,
        Etherblossom,

        // Relics of Nature
        Acorn_of_the_Ages,
        Spiritsong_Stave,
        The_Vesperal_Gem,
        Luneths_Lamp,
        Hagbone_Spite,
        Wychwood_Glaive,

        // Glade specific
        Dawnflask, // Oakenbrow
        Chalice_of_Nectar, // Gnarlroot
        Horn_of_the_Consort, // Heartwood
        Ironbark_Talisman, // Ironbark
        Frozen_Kernel, // Winterleaf
        Jewel_of_Withering, // Dreadwood
        The_Silent_Sickle, // Harvestboon
    };

    enum class Lore : int {
        None,

        // Lore of the Deepwood
        Throne_of_Vines,
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

        SylvanethBase() = default;

        ~SylvanethBase() override = default;

        void setGlade(Glade glade);

        void setArtefact(Artefact artefact);
        void setCommandTrait(CommandTrait commandTrait);

    protected:
        SylvanethBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls battleshockRerolls() const override;

        int braveryModifier() const override;

    protected:

        Glade m_glade = Glade::None;
        Artefact m_artefact = Artefact::None;
        CommandTrait m_commandTrait = CommandTrait::None;
    };

    void Init();

} // namespace Sylvaneth

#endif // SYLVANETHBASE_H