/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ISHARANNSOULRENDER_H
#define ISHARANNSOULRENDER_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class IsharannSoulrender : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        IsharannSoulrender();

        ~IsharannSoulrender() override = default;

        bool configure();

    protected:


    private:

        Weapon m_talunhook,
                m_bill;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lurelight                        TODO
// Hangman's Knot                   TODO
//

} // namespace IdonethDeepkin

#endif // ISHARANNSOULRENDER_H
