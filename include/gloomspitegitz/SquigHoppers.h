/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class SquiqHoppers : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SquiqHoppers(Allegiance allegiance, int numModels, int points);

        ~SquiqHoppers() override = default;

    protected:

        Weapon m_fangFilledGob,
                m_slitta,
                m_slittaBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Boing! Boing! Boing!             TODO
//

} // namespace GloomspiteGitz
