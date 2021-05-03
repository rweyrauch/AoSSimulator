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

    class SporesmashaFanatics : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SporesmashaFanatics(int numModels, int points);

        ~SporesmashaFanatics() override = default;

    private:

        Weapon m_ballAndChain;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Puffshroom Frenzy                TODO
// Spore Cloud                      TODO
// Splat!                           TODO
// Whirling Death                   TODO
//

} // namespace GloomspiteGitz
