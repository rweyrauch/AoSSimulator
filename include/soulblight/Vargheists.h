/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {
    class Vargheists : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Vargheists(CursedBloodline bloodline, int numModels, int points);

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
// Death's Descent                  TODO
//

} //namespace Soulblight
