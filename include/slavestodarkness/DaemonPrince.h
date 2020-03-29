/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAEMONPRINCE_H
#define DAEMONPRINCE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class DaemonPrince : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DaemonPrince();

        ~DaemonPrince() override = default;

        bool configure();

    protected:


    private:

        Weapon m_axe,
            m_sword,
            m_talons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bounding Charge                  TODO
// Hellforged Sword                 TODO
// Immortal Champion                TODO
// Blookslick Ground                TODO
// Arcane Influence                 TODO
// Bloated Blessings                TODO
// Revel in Agony                   TODO
//

} // SlavesToDarkness

#endif //DAEMONPRINCE_H
