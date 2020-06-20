/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINETHREALMLORDS_H
#define LUMINETHREALMLORDS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace LuminethRealmLords {

    enum class CommandTrait : int {
        None,
    };

    enum class Artefact : int {
        None,
    };

    enum class Lore : int {
        None,
    };

    class LuminethBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        LuminethBase() = default;

        ~LuminethBase() override = default;

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
    protected:

        LuminethBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
//

    void Init();

} // namespace LuminethRealmLords

#endif // LUMINETHREALMLORDS_H
