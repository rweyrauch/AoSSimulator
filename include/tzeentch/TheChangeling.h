/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THECHANGELING_H
#define THECHANGELING_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class TheChangeling : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    TheChangeling();
    ~TheChangeling() override = default;

    bool configure();

protected:

private:

    Weapon m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Puckish Misdirection             No
// Arch-deceiver                    No
//

} // namespace Tzeentch

#endif //THECHANGELING_H