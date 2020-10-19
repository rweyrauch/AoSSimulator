/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SONSOFBEHEMAT_H
#define SONSOFBEHEMAT_H

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
        Old_and_Gnarly,
        Louder_than_Words,
        Strong_Right_Foot,
        Very_Acquisitive,
        Extremely_Intimidating,

        // Veterans of One-Sided Battle
        // Monstrously_Tough,
        // Old_and_Gnarly,
        // Louder_than_Words,
        Inescapable_Grip,
        Very_Shouty,
        Eager_for_the_Fight,

        // Unstoppable Besiegers
        // Monstrously_Tough,
        // Old_and_Gnarly,
        // Louder_than_Words,
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
        Jaws_of_the_Mogalodon,
        Wallopin_Tentacle,
        Jar_of_Burny_Grog,
        Net_of_the_Beast_Reaver,
        Glowy_Lantern,
        Krakenskin_Sandals,

        // Trappings of the Titan
        Ironweld_Cestus,
        Club_of_the_First_Oak,
        Mantle_of_the_Destroyer,

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

        Bossy_Pants_and_Clever_Clogs,
        Idiots_with_Flags,
        Shiny_Uns,
        Crowds,
        Wannabes,
        Piggybackers
    };

    class SonsOfBehematBase : public Unit {
    public:
        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SonsOfBehematBase() = default;

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
// Net of the Beast-Reaver          TODO
// More Stuff For Me Collection     TODO
//

    void Init();

} // namespace SonsOfBehemat

#endif //SONSOFBEHEMAT_H