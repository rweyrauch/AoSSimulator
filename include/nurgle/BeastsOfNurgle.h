/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class BeastsOfNurgle : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BeastsOfNurgle(PlagueLegion legion, int numModels, int points);

        ~BeastsOfNurgle() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_limbsAndMaw{Weapon::Type::Melee, "Clawed Limbs and Fanged Maw", 1, 4, 4, 3, -1, 2},
                m_tentaclesAndTongue{Weapon::Type::Melee, "Tentacles and Slobbering Tongue", 1, RAND_D6, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Acidic Slime Trail               TODO
// Pestilent Battering Rams         TODO
// Attention Seekers                Yes
//

} // Nurgle
