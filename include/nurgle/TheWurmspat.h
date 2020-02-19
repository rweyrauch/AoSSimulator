/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEWURMSPAT_H
#define THEWURMSPAT_H

#include "nurgle/Nurgle.h"

namespace Nurgle
{

class TheWurmspat : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    TheWurmspat();
    ~TheWurmspat() override = default;

    bool configure();

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_blightedWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Virulent Discharge               No
// Blighted Weapons                 No
// Festing Bodyguards               No
//

} // Nurgle

#endif //THEWURMSPAT_H
