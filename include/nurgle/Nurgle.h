/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NURGLE_H
#define NURGLE_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle {

    class NurgleBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        NurgleBase() = default;

        ~NurgleBase() override = default;

    protected:

        NurgleBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}
    };

//
// Abilities                    Implemented
// -------------------------------------------
//

    void Init();

} // namespace Nurgle

#endif //NURGLE_H