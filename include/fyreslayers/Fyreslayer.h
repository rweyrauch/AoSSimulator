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
#include <map>
#include <array>

namespace Fyreslayers {

    enum class Lodge : int {
        None = 0,
        Vostarg,
        Greyfyrd,
        Hermdar,
        Lofnir
    };

    enum class Rune : int {
        None,

        Of_Fury,
        Of_Searing_Heat,
        Of_Awakened_Steel,
        Of_Fiery_Determination,
        Of_Relentless_Zeal,
        Of_Farsight,

        Num_Runes
    };

    enum class CommandTrait : int {
        None,

        // Inheritance of Grimnir
        Fury_Of_The_Fyreslayers,        // Yes
        Honour_Of_The_Ancestors,        // Yes
        Spirit_Of_Grimnir,              // Yes
        Blood_Of_The_Berserker,         // TODO
        Iron_Will_Of_The_Guardian,      // Yes
        Destroyer_Of_Foes,              // Yes

        // Magmic Empowerments
        Fyremantle,                     // Yes
        Wisdom_And_Authority,           // Yes
        Oathsayer,                      // Yes
        Ash_Beard,                      // TODO
        Fyresteel_Weaponsmith,          // Yes
        Master_Priest,                  // TODO

        // Lodge specific
        Fiery_Endurance,                // Vostarg
        Battle_Scarred_Veteran,         // Greyfyrd
        Warrior_Indominate,             // Hermdar
        Explosive_Charge                // Lofnir
    };

    enum class Artefact : int {
        None,

        // Heirlooms of the Lodge
        Magnetised_Runes,
        Draught_Of_Magmalt_Ale,
        Drakeslayer,
        Obsidian_Glowhelm,
        Shimmering_Blade,
        Axe_Of_Grimnir,

        // Artefacts of Wrath and Doom
        Beastslayer,
        Bracers_Of_Ember_Iron,
        Rune_Of_Blazing_Fury,

        // Artefacts of the Forge-temple
        Salamander_Cloak,
        Ash_Cloud_Rune,
        Volatile_Brazier,
        Ancestor_Helm,
        Emberstone_Rune,
        Droth_Helm,

        // Icons of Grimnir
        Icon_Of_The_Ancestors,
        Icon_Of_Grimnirs_Condemnation,
        The_Nulsidian_Icon,

        // Lodge specific
        Vosaxe,             // Vostarg
        Helm_Of_Obsidia,    // Greyfyrd
        Tyrant_Slayer,      // Hermdar
        Igneous_Battle_Throne   // Lofnir
    };

    enum class MountTrait : int {
        None,

        // Magmadroth
        Cinder_Crest_Youngblood,
        Flame_Scale_Youngblood,
        Fire_Claw_Adult,
        Lava_Tongue_Adult,
        Coal_Heart_Ancient,         // TODO
        Ash_Horn_Ancient
    };

    enum class Blessing : int {
        None,

        Molten_Infusion,
        Searing_Heat,
        Prayer_Of_Ash,
        Ember_Storm,
        Prayer_Of_Grimnirs_Fury,
        Gilded_Claws
    };

    class Fyreslayer : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        Fyreslayer() = delete;

        ~Fyreslayer() override = default;

        void setLodge(Lodge lodge);

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

    protected:
        Fyreslayer(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onBeginRound(int battleRound) override;

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int chargeModifier() const override;

        int rollRunDistance() override;

        void onRestore() override;

        void activateRune();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int braveryModifier() const override;

        bool battleshockRequired() const override;

        int moveModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onStartMovement(PlayerId player) override;

        int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    protected:

        Lodge m_lodge = Lodge::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        static Rune s_activeRune;
        static bool s_enhancedRuneActive;
        static std::map<Rune, bool> s_availableRunes;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ur-Gold Runes
//    Rune of Fury                  Yes
//    Rune of Searing Heat          Yes
//    Rune of Awakened Steel        Yes
//    Rune of Fiery Determination   Yes
//    Rune of Relentless Zeal       Yes
//    Rune of Farsight              Yes
// Vostarg
//    Fearsome Surge                Yes
//    Honour Our Ancestors          TODO
//    Fiery Endurance               TODO
// Greyfyrd
//    Spoils of Victory             TODO
//    Expert Cohesion               TODO
//    Battle-scarred Veteran        TODO
// Hermdar
//    Seize by Force                TODO
//    Skull-breakers and Oath-takers    TODO
//    Warrior Indominate            TODO
// Lofnir
//    Venerators of Vulcatrix       TODO
//    Torrent of Magma              TODO
//    Explosive Charge              TODO
//

    void Init();

} // namespace Fyreslayers
