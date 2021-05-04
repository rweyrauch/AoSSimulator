/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {
    class Vargheists : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Vargheists(Legion legion, int numModels, int points);

        ~Vargheists() override = default;

    private:

        Weapon m_fangsAndTalons,
                m_fangsAndTalonsVargoyle;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood-maddened Frenzy            TODO
//

} //namespace Death
