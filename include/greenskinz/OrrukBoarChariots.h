/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKBOARCHARIOTS_H
#define ORRUKBOARCHARIOTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz
{

class OrrukBoarChariots : public Unit
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    OrrukBoarChariots();
    ~OrrukBoarChariots() override = default;

    bool configure(int numModels);

protected:

    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
    void onCharged() override;

private:

    Weapon m_pigstikkaSpears,
        m_warBoarsTusks;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Scythed Wheels                   Yes
// Tusker Charge                    Yes
//

} // namespace Greenskinz

#endif // ORRUKBOARCHARIOTS_H