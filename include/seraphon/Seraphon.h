/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SERAPHON_H
#define SERAPHON_H

#include <Unit.h>
#include <Weapon.h>

namespace Seraphon
{

class SeraphonBase : public Unit
{
public:

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    SeraphonBase() = default;
    ~SeraphonBase() override = default;

protected:

    SeraphonBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

void Init();

} // namespace Seraphon

#endif //SERAPHON_H