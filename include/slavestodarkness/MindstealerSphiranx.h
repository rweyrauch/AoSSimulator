/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class MindstealerSphiranx : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MindstealerSphiranx();

        ~MindstealerSphiranx() override;

    protected:

        void configure();

        int telepathicDread(const Unit *unit);

    private:

        Weapon m_claws,
                m_tail;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Telepathic Dread                 Yes
// Dominate Mind                    TODO
//

} // SlavesToDarkness

