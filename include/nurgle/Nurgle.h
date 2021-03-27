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

namespace Nurgle {

    enum class PlagueLegion : int {
        None,
        Munificent_Wanderers,
        Drowning_Guard,
        Blessed_Sons,
        Drowned_Men
    };

    enum class CommandTrait : int {
        None,

        // Rotbringer
        Grandfathers_Blessing,
        Living_Plague,
        Hulking_Physique,
        Bloated_With_Corruption,
        Avalanch_Of_Rotten_Flesh,
        Resilient,

        // Daemon
        //Grandfathers_Blessing,
        //Living_Plague,
        //Hulking_Physique,
        Tainted_Corruptor,
        Nurgling_Infestation,
        Pestilent_Breath,

        // Mortal
        //Grandfathers_Blessing,
        //Living_Plague,
        //Hulking_Physique,
        Hideous_Visage,
        Overpowering_Stench,
        Virulent_Contagion,

        // Legion
        One_Last_Gift, // Munificent Wanderers
        Rotwing_Commander, // Droning Guard
        Degraded_And_Defiled, // Blessed Sons
        Kneel_Before_The_Plague, // Drowned Men
    };

    enum class Artefact : int {
        None,

        // Rotbringer
        The_Splithorn_Helm,
        Muttergrub,
        Rustfang,
        Flesh_Pealer,
        The_Bileheart,
        The_Fecund_Flask,

        // Daemon
        Noxious_Nexus,
        Nurgles_Nail,
        The_Bountiful_Swarm,
        The_Witherstave,
        Tome_Of_A_Thousand_Poxes,
        The_Endless_Gift,

        // Mortal
        The_Virulent_Blade,
        The_Foetid_Shroud,
        Sublucus_Stenchplate,
        The_Eye_Of_Nurgle,
        The_Carrion_Dirge,
        The_Shield_Of_Growths,

        // Legion
        Mucktalon, // Munificent Wanderers
        Cloak_Of_Flies, // Drowning Guard
        Blotshell_Bileplate, // Blessed Sons
        Rot_Kraken_Hide, // Drowned Men
    };

    enum class Lore : int {
        None,

        // Rotbringer
        Blades_Of_Putrification,
        Rancid_Visitations,
        Gift_Of_Contagion,

        // Daemon
        Favoured_Poxes,
        Glorious_Afflictions,
        Sumptuous_Pestilence,

        // Mortal
        Magnificent_Buboes,
        Plague_Squall,
        Cloying_Quagmire,
    };

    enum class CycleOfCorruption : int {
        Corrupted_Regrowth = 0,
        Unnatural_Vitality,
        Fecund_Vigour,
        The_Burgeoning,
        Plague_Of_Misery,
        Nauseous_Revulsion,
        Rampant_Disease
    };

    class NurgleBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        NurgleBase() = default;

        ~NurgleBase() override = default;

        void setLegion(PlagueLegion legion);

        void setCommandTrait(CommandTrait trait) { m_commandTrait = trait; }

        void setArtefact(Artefact artefact) { m_artefact = artefact; }

    protected:

        NurgleBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onStartHero(PlayerId player) override;

        int moveModifier() const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    protected:

        PlagueLegion m_plagueLegion = PlagueLegion::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        static CycleOfCorruption s_currentCorruption;
        static bool s_usedGrandfathersBlessing;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cycle of Corruption              TODO
//    Corrupted Regrowth            Yes
//    Unatural Vitality             Yes
//    Fecund Vigour                 Yes
//    The Burgeoning                TODO
//    Plague of Misery              TODO
//    Nauseous Revulsion            TODO
//    Rampant Disease               TODO
// The Garden of Nurgle             TODO
// Summon Daemons of Nurgle         TODO
// Command Traits
//    Grandfathers_Blessing         TODO
//    Living_Plague                 Yes
//    Hulking_Physique              Yes
//    Bloated_With_Corruption       TODO
//    Avalanch_Of_Rotten_Flesh      Yes
//    Resilient                     Yes
//    Tainted_Corruptor             TODO
//    Nurgling_Infestation          TODO
//    Pestilent_Breath              TODO
//    Hideous_Visage                TODO
//    Overpowering_Stench           TODO
//    Virulent_Contagion            Yes
// Artefacts
//    The_Splithorn_Helm            TODO
//    Muttergrub                    TODO
//    Rustfang                      TODO
//    Flesh_Pealer                  TODO
//    The_Bileheart                 TODO
//    The_Fecund_Flask              TODO
//    Noxious_Nexus                 TODO
//    Nurgles_Nail                  TODO
//    The_Bountiful_Swarm           TODO
//    The_Witherstave               TODO
//    Tome_Of_A_Thousand_Poxes      TODO
//    The_Endless_Gift              TODO
//    The_Virulent_Blade            TODO
//    The_Foetid_Shroud             TODO
//    Sublucus_Stenchplate          TODO
//    The_Eye_Of_Nurgle             TODO
//    The_Carrion_Dirge             TODO
//    The_Shield_Of_Growths         TODO
// The Munificent Wanderers
//   Locus of Corrosion             TODO
//   Infested with Wonders          TODO
//   One Last Gift                  TODO
//   Mucktalon                      TODO
// The Droning Guard
//   Locus of Corrosion             TODO
//   Twice-blessed Rotspawn         TODO
//   Rotwing Commander              TODO
//   Cloak of Flies                 TODO
// The Blessed Sons
//   Nurgle's Embrace               TODO
//   Foul Conqueror                 TODO
//   Degraded and Defiled           TODO
//   Blotshell Bileplate            TODO
// The Drowned Men
//   Nurgle's Embrace               TODO
//   Bloated Raider                 TODO
//   Kneel Before the Plague!       TODO
//   Rot-kraken Hide                TODO
//

    void Init();

} // namespace Nurgle
