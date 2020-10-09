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

    //
    // Command Traits
    //
    enum class CommandTrait : int {
        None,
    };

    //
    // Artefact
    //
    enum class Artefact : int {
        None,
    };

    class SonsOfBehematBase : public Unit {
    public:
        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SonsOfBehematBase() = default;

        ~SonsOfBehematBase() override = default;

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:

        SonsOfBehematBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Son of Behemat                   TODO
// Timberrrrr!                      TODO
// Mightier Make Rightier           TODO
// Getting Stuck In                 TODO
// Grab Those Rock And Chuck 'Em    TODO
// Get a Move On, You Slackers!     TODO
// Fierce Loathings
//   Bossy Pants and Clever Cogs    TODO
//   Idiots with Flag               TODO
//   Shiny 'Uns                     TODO
//   Crowds                         TODO
//   Wannabes                       TODO
//   Piggybackers                   TODO
// Breaking Down the Houses         TODO
// Get Rid of 'Em!                  TODO
// Net of the Beast-Reaver          TODO
// More Stuff For Me Collection     TODO
//

    void Init();

} // namespace SonsOfBehemat

#endif //SONSOFBEHEMAT_H