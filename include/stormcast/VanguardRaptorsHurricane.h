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

    class VanguardRaptorsHurricane : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        VanguardRaptorsHurricane(Stormhost stormhost, int numModels, int points);

        ~VanguardRaptorsHurricane() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_hurricaneCrossbow,
                m_hurricaneCrossbowPrime,
                m_heavyStock;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rapid Fire                       Yes
// Suppressing Fire                 TODO
//

} // namespace StormcastEternals
