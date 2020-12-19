/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPOILPOXSCRIVENER_H
#define SPOILPOXSCRIVENER_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class SpoilpoxScrivenerHeraldOfNurgle : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpoilpoxScrivenerHeraldOfNurgle();

        ~SpoilpoxScrivenerHeraldOfNurgle() override;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Rerolls keepCountingChargeRerolls(const Unit *unit);

        Rerolls keepCountingToHitRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target);

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

#endif //SPOILPOXSCRIVENER_H