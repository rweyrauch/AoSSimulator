/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kruleboyz/KruleBoyz.h>

namespace Kruleboyz {

    class KillabossOnGnashtoof : public Kruleboyz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KillabossOnGnashtoof(Warclan warclan, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~KillabossOnGnashtoof() override = default;

    protected:

    private:

        Weapon  m_stikka{Weapon::Type::Melee, "Jagged Boss-Stikka", 1, 4, 3, 3, -1, 2},
                m_fangs{Weapon::Type::Melee, "Bone-Crushing Fangs", 1, 4, 3, 3, -2, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace Kruleboyz
