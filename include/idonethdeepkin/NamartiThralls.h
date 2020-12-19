/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NAMARTITHRALLS_H
#define NAMARTITHRALLS_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class NamartiThralls : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        NamartiThralls();

        ~NamartiThralls() override = default;

        bool configure(int numModels, int numIconBearers);

    private:

        Weapon m_lanmariBlade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sweeping Blows                   TODO
//

} // namespace IdonethDeepkin

#endif // NAMARTITHRALLS_H
