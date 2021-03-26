/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {
    class SavageBigBoss : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        SavageBigBoss();

        ~SavageBigBoss() override = default;

    protected:

        void configure();

    private:

        Weapon m_chompasBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Let Me at 'Em                    TODO
// Savage Attack                    Yes
//

} // namespace Bonesplitterz
