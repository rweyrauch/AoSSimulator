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

    class KnightZephyros : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        KnightZephyros();

        ~KnightZephyros() override = default;

    protected:

        void configure();

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_boltstormPistol,
                m_tempestAxes;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lightning-fast Strikes           Yes
// Tireless Hunter                  Yes
// Windrider                        TODO
//

} // namespace StormcastEternals
