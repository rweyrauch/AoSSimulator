/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEDDRASKULLSCRYER_H
#define THEDDRASKULLSCRYER_H


#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class TheddraSkullscryer : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheddraSkullscryer();

        ~TheddraSkullscryer() override = default;

        bool configure();

    protected:


    private:

        Weapon m_wand;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pact of Soul and Iron            TODO
// Enfeeblement                     TODO
//

} // SlavesToDarkness

#endif //THEDDRASKULLSCRYER_H
