/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NAMARTIREAVERS_H
#define NAMARTIREAVERS_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class NamartiReavers : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        NamartiReavers();

        ~NamartiReavers() override = default;

        bool configure(int numModels, int numIconBearers);

    protected:

        void onStartShooting(PlayerId player) override;

        // Swift Tide
        Rerolls runRerolls() const override { return Reroll_Failed; }

    protected:

        int m_numIconBearers = 0;

    private:

        Weapon m_keeningBlade,
                m_whisperbowAimedFire,
                m_whisperbowStormFire;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Swift Tide                       Yes
// Fluid Firing Style               Yes
//

} // namespace IdonethDeepkin

#endif // NAMARTIREAVERSS_H
