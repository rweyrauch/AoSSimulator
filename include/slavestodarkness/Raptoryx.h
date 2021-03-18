/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Raptoryx : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Raptoryx();

        ~Raptoryx() override = default;

        bool configure(int numModels);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_beakAndTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crazed Flock                     Yes
//

} // SlavesToDarkness
