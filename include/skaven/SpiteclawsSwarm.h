/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class SpiteclawsSwarm : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpiteclawsSwarm();

        ~SpiteclawsSwarm() override = default;

    protected:

        void configure();

    private:

        Weapon m_stabbingBlade,
                m_rustyFlail,
                m_rustySpear,
                m_punchDaggers;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aversion to Death                TODO
//

} // namespace Skaven
