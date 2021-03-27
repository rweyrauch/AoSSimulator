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

    class LordCelestant : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        LordCelestant();

        ~LordCelestant() override = default;

    protected:

        void configure();

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override {
            int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);
            // Inescapable Vengeance
            if (m_charged) attacks += 1;
            return attacks;
        }

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_runeblade,
                m_warhammer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Inescapable Vengeance            Yes
// Sigmarite Warcloak               Yes
// Furious Retribution              Yes
//

} // namespace StormcastEternals
