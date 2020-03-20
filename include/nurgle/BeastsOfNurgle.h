/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BEASTSOFNURGLE_H
#define BEASTSOFNURGLE_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class BeastsOfNurgle : public NurgleBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BeastsOfNurgle();
    ~BeastsOfNurgle() override = default;

    bool configure(int numModels);

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_clawsAndTentacles,
        m_slobberingTongue;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Acidic Slime Trail               TODO
// Locus of Virulence               Yes
// Attention Seekers                Yes
//

} // Nurgle

#endif //BEASTSOFNURGLE_H