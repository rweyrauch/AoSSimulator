/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHOUNDS_H
#define WARHOUNDS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class ChaosWarhounds : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        ChaosWarhounds();

        ~ChaosWarhounds() override = default;

        bool configure(int numModels);

    protected:

        int rollRunDistance() const override;

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

#endif //WARHOUNDS_H