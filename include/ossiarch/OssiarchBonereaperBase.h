/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OSSIARCHBASE_H
#define OSSIARCHBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class OssiarchBonereaperBase : public Unit
{
public:
    OssiarchBonereaperBase() = default;
    ~OssiarchBonereaperBase() override = default;

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

protected:
    OssiarchBonereaperBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

void Init();

} // namespace OssiarchBonereapers

#endif // OSSIARCHBASE_H