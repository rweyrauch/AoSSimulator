/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLOPPITYBILEPIPER_H
#define SLOPPITYBILEPIPER_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class SloppityBilepiperHeraldOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SloppityBilepiperHeraldOfNurgle();
    ~SloppityBilepiperHeraldOfNurgle() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

    int diseaseOfMirthBraveryMod(const Unit* unit);
    Rerolls jollyGutpipesChargeReroll(const Unit* unit);

private:

    Weapon m_marotter;

    lsignal::slot m_diseaseOfMirthSlot, m_jollyGutpipesSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Disease of Mirth                 Yes
// Jolly Gutpipes                   Yes
//

} // Nurgle

#endif //SLOPPITYBILEPIPER_H