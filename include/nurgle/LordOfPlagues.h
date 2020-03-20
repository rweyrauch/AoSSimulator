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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Wanton Slaughter                 TODO
// Rotten Corpse Mulch              TODO
// Plague-ridden Great Weapon       Yes
// Grandfather's Gift               TODO
//

} // Nurgle

#endif //LORDOFPLAGUES_H