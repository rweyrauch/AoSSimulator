/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAWTRIBESBASE_H
#define MAWTRIBESBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class MawtribesBase : public Unit
{
public:
    MawtribesBase() = default;
    ~MawtribesBase() override = default;

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

protected:
    MawtribesBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

void Init();

} // namespace OgorMawtribes

#endif // MAWTRIBESBASE_H