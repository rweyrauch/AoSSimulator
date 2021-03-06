/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ironjawz/Ironjawz.h>

namespace Ironjawz {

    class OrrukMegaboss : public Ironjawz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OrrukMegaboss();

        ~OrrukMegaboss() override = default;

        bool configure();

    protected:

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_bossChoppaAndFist;
        int m_modelsSlainAtStartOfCombat = 0;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Go on Ladz, Get Stuck In!        Yes
// Strength from Victory            Yes
// Rip-toof Fist                    Yes
//

} // namespace Ironjawz
