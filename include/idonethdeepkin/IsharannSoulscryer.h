/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SOULSCRYER_H
#define SOULSCRYER_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class IsharannSoulscryer : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        IsharannSoulscryer();

        ~IsharannSoulscryer() override = default;

        bool configure();

    protected:


    private:

        Weapon m_shoal,
                m_claw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Find of Ways                     TODO
// Seeker of Souls                  TODO
//

} // namespace IdonethDeepkin

#endif // SOULSCRYER_H
