/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPOILPOX_H
#define SPOILPOX_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class SpoilpoxScrivenerHeraldOfNurgle : public NurgleBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SpoilpoxScrivenerHeraldOfNurgle();
    ~SpoilpoxScrivenerHeraldOfNurgle() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

    Rerolls keepCountingChargeRerolls(const Unit* unit);
    Rerolls keepCountingToHitRerolls(const Unit* attacker, const Weapon* weapon, const Unit* target);

private:

    Weapon m_sneeze,
        m_maw;

    lsignal::slot m_keepCountingChargeSlot, m_keepCountingToHitSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Keep Counting, I'm Watching You  Yes
//

} // Nurgle

#endif //SPOILPOX_H