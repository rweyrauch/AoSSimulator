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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Virulent Discharge               TODO
// Blighted Weapons                 TODO
// Festing Bodyguards               TODO
//

} // Nurgle

#endif //THEWURMSPAT_H
