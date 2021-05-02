/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class NamartiReavers : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        NamartiReavers(int points);

        ~NamartiReavers() override = default;

        bool configure(int numModels, int numIconBearers);

    protected:

        void onStartShooting(PlayerId player) override;

        // Swift Tide
        Rerolls runRerolls() const override { return Rerolls::Failed; }

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

