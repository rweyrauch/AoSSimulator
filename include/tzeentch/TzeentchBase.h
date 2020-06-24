/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZEENTCHBASE_H
#define TZEENTCHBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Tzeentch {

    enum class ChangeCoven : int {
        None,
        EternalConflagration,
        HostsDuplicitous,
        HostsArcanum,
        CultOfTheTransientForm,
        PyrofaneCult,
        GuildOfSummoners
    };

    enum class CommandTrait : int {
        None,

        // Arcanites
        Arch_Sorcerer,
        Nexus_of_Fate,
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
        Will_of_the_Phantom_Lord,   // Host Duplicitous
        Spell_Hunters,  // Hosts Arcanum
        Defiant_in_their_Pursuit,   // Transient Form
        Shrouded_in_Unnatural_Flame,    // Pyrofane Cult
        Prophet_of_the_Ostensible  // Guild of Summoners
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
        Blade_of_Fate,
        Souleater,
        Phatasmal_Weapons,
        Pyrofyre_Stave,
        Aura_of_Mutabulity,
        Wellspring_of_Arcane_Might,
        Aspect_of_Tzeentch,

        // Coven specific
        Shroud_of_Warpflame, // Eternal Conflaguration
        Brand_of_the_Split_Daemon,   // Host Duplicitous
        The_Fanged_Circlet, // Hosts Arcanum
        Chaotica_Amulet,    // Transient Form
        Chainfire_Amulet,   // Pyrofane Cult
        Brimstone_Familiar // Guild of Summoners
    };

    enum class Lore : int {
        // Lore of Fate
        Bolt_of_Tzeentch,
        Arcane_Suggestion,
        Glimpse_the_Future,
        Shield_of_Faith,
        Infusion_Arcanum,
        Treacherous_Bond,

        // Lore of Change
        //Bolt_of_Tzeentch,
        Treason_of_Tzeentch,
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

    private:

        ChangeCoven m_coven = ChangeCoven::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

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
//    Coruscating Flames            TODO
// The Hosts Duplicitous
//    Ranks of Mischievous Mirages  TODO
//    Impossible to Anticipate      TODO
//    Will of the Phantom Lord      TODO
// The Hosts Arcanum
//    Thieves of all Things Arcane  TODO
//    Entourage of Sky-sharks       TODO
//    Spell Hunters                 TODO
// The Cult of the Transient Form
//    The Change-gift               TODO
//    Fate of Transmutation         TODO
//    Defiant in their Pursuit      TODO
// The Pyrofane Cult
//    Arrows of Tzeentch            Partial
//    Immolate                      TODO
//    Shrouded in Unnatural Flame   TODO
// The Guild of Summoners
//    Scions of the Exiled          TODO
//    Will of the Arcane Lords      TODO
//    Prophet of the Ostensible     TODO
//

} // Tzeentch

#endif //TZEENTCHBASE_H