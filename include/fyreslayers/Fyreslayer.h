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
        Of_Fury,
        Of_Searing_Heat,
        Of_Awakened_Steel,
        Of_Fiery_Determination,
        Of_Relentless_Zeal,
        Of_Farsight
    };

    enum class CommandTrait : int {
        None,

        // Inheritance of Grimnir
        Fury_Of_The_Fyreslayers,
        Honour_Of_The_Ancestors,
        Spirit_Of_Grimnir,
        Blood_Of_The_Berserker,
        Iron_Will_Of_The_Guardian,
        Destroyer_Of_Foes,

        // Magmic Empowerments
        Fyremantle,
        Wisdom_And_Authority,
        Oathsayer,
        Ash_Beard,
        Fyresteel_Weaponsmith,
        Master_Priest,

        // Lodge specific
        Fiery_Endurance,    // Vostarg
        Battle_Scarred_Veteran, // Greyfyrd
        Warrior_Indominate, // Hermdar
        Explosive_Charge    // Lofnir
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
        Coal_Heart_Ancient,
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

        Fyreslayer() = default;

        ~Fyreslayer() override = default;

        void setLodge(Lodge lodge);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        Fyreslayer(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int chargeModifier() const override;

        int rollRunDistance() const override;

        void activateRune();

    protected:

        Lodge m_lodge = Lodge::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        bool m_activatedRune = false;
        std::map<Rune, bool> m_availableRunes;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ur-Gold Runes
//    Rune of Fury                  TODO
//    Rune of Searing Heat          TODO
//    Rune of Awakened Steel        TODO
//    Rune of Fiery Determination   TODO
//    Rune of Relentless Zeal       TODO
//    Rune of Farsight              TODO
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
