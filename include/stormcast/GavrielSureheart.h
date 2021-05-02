/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class GavrielSureheart : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GavrielSureheart(bool isGeneral);

        ~GavrielSureheart() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        Weapon m_starboundBlade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Inescapable Vengeance            Yes
// Sigmarite Thundershield          Yes
// Once More, For Sigmar, Charge!   Yes
//

} // namespace StormcastEternals
