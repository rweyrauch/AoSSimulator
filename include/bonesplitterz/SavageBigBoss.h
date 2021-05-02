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

        static int ComputePoints(const ParameterList& parameters);

        SavageBigBoss(Warclan warclan, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~SavageBigBoss() override = default;

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
