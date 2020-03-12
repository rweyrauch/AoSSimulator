/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THANQUALONBONERIPPER_H
#define THANQUALONBONERIPPER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class ThanquolOnBoneripper : public Skaventide
{
public:
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 400;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ThanquolOnBoneripper();
    ~ThanquolOnBoneripper() override = default;

    bool configure();

protected:

private:

    Weapon m_projectors,
        m_staff,
        m_braziers,
        m_blows;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Protection of the Horned Rat     No
// Staff of the Horned Rat          No
// Warp-amulet                      No
// Warpfire Braziers                No
// Warpfire Projectors              No
// Warpstone Addiction              No
// Madness                          No
// Power Behind the Throne          No

} // namespace Skaven

#endif //THANQUALONBONERIPPER_H