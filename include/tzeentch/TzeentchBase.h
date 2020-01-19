/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZEENTCHBASE_H
#define TZEENTCHBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class TzeentchBase : public Unit
{
public:

    enum ChangeCoven
    {
        None,
        EternalConflagration,
        HostsDuplicitous,
        HostsArcanum,
        CultOfTheTransientForm,
        PyrofaneCult,
        GuildOfSummoners
    };

    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    TzeentchBase() = default;
    ~TzeentchBase() override = default;

    void setChangeCoven(ChangeCoven coven);

protected:

    TzeentchBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

private:

    ChangeCoven m_coven = None;
};

void Init();

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

} // Tzeentch

#endif //TZEENTCHBASE_H