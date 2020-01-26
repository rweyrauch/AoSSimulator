/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEGIONOFNAGASH_H
#define LEGIONOFNAGASH_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class LegionOfNagashBase : public Unit
{
public:

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    LegionOfNagashBase() = default;
    ~LegionOfNagashBase() override = default;

protected:

    LegionOfNagashBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

void Init();

} // namespace Death

#endif //LEGIONOFNAGASH_H