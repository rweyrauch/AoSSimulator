/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Kruleboyz {

    enum class Warclan : int {
        None,
    };

    enum class CommandTrait : int {
        None,
    };

    enum class Artefact : int {
        None,
    };

    enum class MountTrait : int {
        None,
    };

    enum class Lore : int {
        None,
    };

    class Kruleboyz : public Unit {
    public:
        Kruleboyz() = delete;

        ~Kruleboyz() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

    protected:
        Kruleboyz(Warclan warclan, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
        }

    protected:

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
        MountTrait m_mountTrait = MountTrait::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
//

    void Init();

} // namespace Kruleboyz
