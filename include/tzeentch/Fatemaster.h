/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FATEMASTER_H
#define FATEMASTER_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class Fatemaster : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Fatemaster();
    ~Fatemaster() override = default;

    bool configure();

protected:

private:

    Weapon m_glaive,
        m_teethAndHorns;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hovering Disc of Tzeentch        No
// Soulbound Shield                 No
// Lord of Fate                     No
//

} // namespace Tzeentch

#endif //FATEMASTER_H
