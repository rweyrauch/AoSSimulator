/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>
#include "../../src/tzeentch/DestinyDice.h"

namespace Tzeentch {

    enum class ChangeCoven : int {
        None,
        Eternal_Conflagration,
        Hosts_Duplicitous,
        Hosts_Arcanum,
        Cult_Of_The_Transient_Form,
        Pyrofane_Cult,
        Guild_Of_Summoners,

        // Subfactions
        Unbound_Flux,
        Cult_Of_A_Thousand_Eyes,
    };

    enum class CommandTrait : int {
        None,

        // Arcanites
        Arch_Sorcerer,
        Nexus_Of_Fate,
        Magical_Supremancy,
        Boundless_Mutation,
        Cult_Demagogue,
        Arcane_Sacrifice,

        // Mortals
        // Nexus_of_Fate,
        Soul_Burn,
        Illusionist,

        // Daemons
        //Arch_Sorcerer,
        //Nexus_of_Fate,
        //Magical_Supremancy,
        Daemonspark,
        Incorporeal_Form,
        Aether_Tether,

        // Coven specific
        Coruscating_Flames,  // Eternal Conflaguration
        Will_Of_The_Phantom_Lord,   // Host Duplicitous
        Spell_Hunters,  // Hosts Arcanum
        Defiant_In_Their_Pursuit,   // Transient Form
        Shrouded_In_Unnatural_Flame,    // Pyrofane Cult
        Prophet_Of_The_Ostensible,  // Guild of Summoners
        Aegis_Of_Insanity, // Unbound Flux
        Tzeentch_Is_Pleased, // Cult of a Thousand Eyes
    };

    enum class Artefact : int {
        None,

        // Arcanites
        Ambitions_End,
        Secret_Eater,
        Spiteful_Shield,

        // Mortals
        Wicked_Shard,
        Changeblade,
        Nexus_Staff,
        Timeslip_Pendant,
        Daemonheart,
        Paradoxical_Shield,

        // Daemons
        Warpfire_Blade,
        Sentient_Weapon,
        Blade_Of_Fate,
        Souleater,
        Phatasmal_Weapons,
        Pyrofyre_Stave,
        Aura_Of_Mutabulity,
        Wellspring_Of_Arcane_Might,
        Aspect_Of_Tzeentch,

        // Coven specific
        Shroud_Of_Warpflame, // Eternal Conflaguration
        Brand_Of_The_Split_Daemon,   // Host Duplicitous
        The_Fanged_Circlet, // Hosts Arcanum
        Chaotica_Amulet,    // Transient Form
        Chainfire_Amulet,   // Pyrofane Cult
        Brimstone_Familiar, // Guild of Summoners
        The_Enlightener, // Unbound Flux
        Crown_Of_Whispers, // Cult of a Thousand Eyes
    };

    enum class Lore : int {
        None,

        // Lore of Fate
        Bolt_Of_Tzeentch,
        Arcane_Suggestion,
        Glimpse_The_Future,
        Shield_Of_Faith,
        Infusion_Arcanum,
        Treacherous_Bond,

        // Lore of Change
        //Bolt_of_Tzeentch,
        Treason_Of_Tzeentch,
        Arcane_Transformation,
        Unchecked_Mutation,
        Fold_Reality,
        Tzeentchs_Firestorm
    };

    class TzeentchBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        TzeentchBase() = default;

        ~TzeentchBase() override = default;

        void setChangeCoven(ChangeCoven coven);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
    protected:

        TzeentchBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onStartHero(PlayerId player) override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        double unbindingDistance() const override;

        Rerolls castingRerolls() const override;

        Rerolls unbindingRerolls() const override;

        int braveryModifier() const override;

        void onRestore() override {
            m_usedDaemonspark = false;
            s_destinyDice.clear();
            s_destinyDice.initialize();
        }

        bool battleshockRequired() const override;

    private:

        ChangeCoven m_coven = ChangeCoven::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        bool m_usedDaemonspark = false;

        static DestinyDice s_destinyDice;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Masters of Destiny               TODO
// Locus of Change                  Yes
// Summon Daemons of Tzeentch       TODO
// Gifts of Worship
//    Mass Conjuration              TODO
//    Ninefold Dismantlement        TODO
//    Overthrow Leaders             TODO
//    Reckless Abandon              TODO
//    Tide of Anarchy               TODO
// The Eternal Conflagration
//    Twisters of Materiality       Yes
//    Infernos of Mutation          TODO
//    Coruscating Flames            Yes
// The Hosts Duplicitous
//    Ranks of Mischievous Mirages  TODO
//    Impossible to Anticipate      TODO
//    Will of the Phantom Lord      Yes
// The Hosts Arcanum
//    Thieves of all Things Arcane  TODO
//    Entourage of Sky-sharks       TODO
//    Spell Hunters                 TODO
// The Cult of the Transient Form
//    The Change-gift               TODO
//    Fate of Transmutation         TODO
//    Defiant in their Pursuit      Yes
// The Pyrofane Cult
//    Arrows of Tzeentch            Partial
//    Immolate                      TODO
//    Shrouded in Unnatural Flame   Yes
// The Guild of Summoners
//    Scions of the Exiled          TODO
//    Will of the Arcane Lords      TODO
//    Prophet of the Ostensible     Yes
// The Unbound Flux
//    Maddening Cascade             TODO
//    Fuelled by Mayhem             TODO
//    Aegis of Insanity             TODO
//    The Enlightener               TODO
// A Cult of a Thousand Eyes
//    Marked for Death              TODO
//    Eyes Everywhere               TODO
//    Tzeentch is Pleased           TODO
//    Crown of Whispers             TODO
// Command Traits
//    Arch_Sorcerer                 TODO
//    Nexus_Of_Fate                 Yes
//    Magical_Supremancy            Yes
//    Boundless_Mutation            Yes
//    Cult_Demagogue                TODO
//    Arcane_Sacrifice              TODO
//    Soul_Burn                     TODO
//    Illusionist                   TODO
//    Daemonspark                   Yes
//    Incorporeal_Form              Yes
//    Aether_Tether                 Yes
//


} // Tzeentch
