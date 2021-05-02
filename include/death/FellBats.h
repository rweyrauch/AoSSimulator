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
    class FellBats : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FellBats(int points);

        ~FellBats() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_fangs;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scent of Gore                    TODO
//

} //namespace Death
