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

    class GryphHounds : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GryphHounds(Stormhost stormhost, int numModels, int points);

        ~GryphHounds() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_beakAndClaws,
                m_beakAndClawsAlpha;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Loyal Companion                  Yes
// Darting Attacks                  TODO
// Warning Cry                      TODO
//

} // namespace StormcastEternals
