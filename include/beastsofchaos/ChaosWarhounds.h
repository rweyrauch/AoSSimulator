/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class ChaosWarhounds : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        ChaosWarhounds() = delete;

        ~ChaosWarhounds() override = default;

        ChaosWarhounds(Greatfray fray, int numModels, int points);

    protected:

        int rollRunDistance() override;

    private:

        Weapon m_slaveringJaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Outrunners of Chaos              Yes
//

} // namespace BeastsOfChaos
