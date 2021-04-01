/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <array>

namespace SonsOfBehemat {

    enum class Tribe : int {
        Taker,
        Stomper,
        Breaker
    };

    //
    // Command Traits
    //
    enum class CommandTrait : int {
        None,

        // Irascible Old Bullies
        Monstrously_Tough,
        Old_And_Gnarly,
        Louder_Than_Words,
        Strong_Right_Foot,
        Very_Acquisitive,
        Extremely_Intimidating,

        // Veterans of One-Sided Battle
        // Monstrously_Tough,
        // Old_And_Gnarly,
        // Louder_Than_Words,
        Inescapable_Grip,
        Very_Shouty,
        Eager_For_The_Fight,

        // Unstoppable Besiegers
        // Monstrously_Tough,
        // Old_And_Gnarly,
        // Louder_Than_Words,
        Extremely_Bitter,
        Smasher,
        Sees_Red
    };

    //
    // Artefact
    //
    enum class Artefact : int {
        None,

        // Trophies Taken by Force
        Jaws_Of_The_Mogalodon,
        Wallopin_Tentacle,
        Jar_Of_Burny_Grog,
        Net_Of_The_Beast_Reaver,
        Glowy_Lantern,
        Krakenskin_Sandals,

        // Trappings of the Titan
        Ironweld_Cestus,
        Club_Of_The_First_Oak,
        Mantle_Of_The_Destroyer,

        // Legacy of Demolition
        Enchanted_Portcullis,
        The_Great_Wrecka,
        Kingslaughter_Cowl
    };

    //
    // Fierce Loathings
    //
    enum class FierceLoathing : int {
        None,

        Bossy_Pants_And_Clever_Clogs,
        Idiots_With_Flags,
        Shiny_Uns,
        Crowds,
        Wannabes,
        Piggybackers
    };

    class SonsOfBehematBase : public Unit {
    public:
        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SonsOfBehematBase() = delete;

        ~SonsOfBehematBase() override = default;

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

        void setTribe(Tribe tribe) {
            m_tribe = tribe;
        }

        void setFierceLoating(FierceLoathing loathing) {
            m_fierceLoathing = loathing;
        }

    protected:

        SonsOfBehematBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int woundModifier() const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    protected:

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
        Tribe m_tribe = Tribe::Taker;
        FierceLoathing m_fierceLoathing = FierceLoathing::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mightier Make Rightier           TODO
// Chuck Rocks                      Yes
// Big Shouts
//   Get a Move On, You Slackers!               TODO
//   Grab Those Rock And Chuck 'Em at Somethin! TODO
//   Oi, You! Yes, You! Charge!                 TODO
//   Stop Muckin' About and Hit 'Em!            TODO
//   Watch Yer Backs, Your Gormless Lot!        TODO
//   Where Do You Think You're Going?           TODO
// Fierce Loathings
//   Bossy Pants and Clever Cogs    Yes
//   Idiots with Flag               Yes
//   Shiny 'Uns                     Yes
//   Crowds                         Yes
//   Wannabes                       Yes
//   Piggybackers                   Yes
// Get Rid of 'Em!                  TODO
// Monstrously Tough                Yes
// Old and Gnarly                   Yes
// Louder than Words                Yes
// Irascible Old Bullies
//   Strong Right Foot              TODO
//   Very Acquisitive               TODO
//   Extremely Intimidating         Yes
// Veterans of One-Sided Battle
//   Inescapable Grip               TODO
//   Very Shouty                    Yes
//   Eager for the Fight            TODO
// Unstoppable Besiegers
//   Extremely Bitter               TODO
//   Smasher                        TODO
//   Sees_Red                       TODO
// Net of the Beast-Reaver          TODO
// More Stuff For Me Collection     TODO
//

    void Init();

} // namespace SonsOfBehemat
