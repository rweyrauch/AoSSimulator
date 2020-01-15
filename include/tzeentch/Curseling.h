/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CURSELING_H
#define CURSELING_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class CurselingEyeOfTzeentch : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    CurselingEyeOfTzeentch();
    ~CurselingEyeOfTzeentch() override = default;

    bool configure();

protected:

private:

    Weapon m_sword,
        m_flail,
        m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Disrupter of the Arcane          No
// Vessel of Chaos                  No
// Glean Magic                      No
//

} // namespace Tzeentch

#endif //CURSELING_H
