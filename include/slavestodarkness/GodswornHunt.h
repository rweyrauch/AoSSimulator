/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GODSWORNHUNT_H
#define GODSWORNHUNT_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class GodswornHunt : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GodswornHunt();

        ~GodswornHunt() override = default;

        bool configure();

    protected:


    private:

        Weapon m_huntingBow,
            m_javelin,
            m_knife,
            m_greatWeapon,
            m_bowMelee,
            m_bite;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pact of Soul and Iron            TODO
//

} // SlavesToDarkness

#endif //GODSWORNHUNT_H
