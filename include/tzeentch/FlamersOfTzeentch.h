/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class FlamersOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FlamersOfTzeentch(int points);

        ~FlamersOfTzeentch() override = default;

        bool configure(int numModels);

    protected:

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_warpflame,
                m_warpflamePyrocaster,
                m_flamingMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Capricious Warpflame             Yes
// Touched by Fire                  Yes
// Guided by Billowing Flames       Yes
//

} // Tzeentch
