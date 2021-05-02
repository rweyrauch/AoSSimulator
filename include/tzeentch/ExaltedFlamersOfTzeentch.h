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

    class ExaltedFlamersOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ExaltedFlamersOfTzeentch(int points);

        ~ExaltedFlamersOfTzeentch() override = default;

        bool configure(int numModels);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        Weapon m_warpflame,
                m_flamingMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Capricious Warpflame             Yes
// Touched by Fire                  Yes
//

} // Tzeentch
