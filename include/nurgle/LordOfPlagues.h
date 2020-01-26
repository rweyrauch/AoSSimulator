/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFPLAGUES_H
#define LORDOFPLAGUES_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class LordOfPlagues : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    LordOfPlagues();
    ~LordOfPlagues() override = default;

    bool configure();

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

protected:


private:

    Weapon m_plagueriddenGreatBlade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Wanton Slaughter                 No
// Rotten Corpse Mulch              No
// Plagueridden Great Weapon        Yes
//

} // Nurgle

#endif //LORDOFPLAGUES_H