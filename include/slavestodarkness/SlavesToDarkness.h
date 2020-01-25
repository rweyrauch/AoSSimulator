/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAVESTODARKNESS_H
#define SLAVESTODARKNESS_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class SlavesToDarknessBase : public Unit
{
public:

    enum DamnedLegion {
        Ravagers,
        Cabalists,
        Despoilers,
        HostOfTheEverchosen
    };

    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    SlavesToDarknessBase();
    ~SlavesToDarknessBase() override = default;

    void setDamnedLegion(DamnedLegion legion);

protected:

    SlavesToDarknessBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

private:

    DamnedLegion m_legion = Ravagers;
};

void Init();

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Aura of Chaos
//    Aura of Khorne                No
//    Aura of Tzeentch              No
//    Aura of Nurgle                No
//    Aura of Slaanesh              No
//    Aura of Chaos Undivided       No
// Eye of the Gods                  No
// Ravagers                         No
// Cabalists                        No
// Despoilers                       No
// Host of the Everchosen           No

} // SlavesToDarkness

#endif //SLAVESTODARKNESS_H