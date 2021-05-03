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

    class SquigGobba : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SquigGobba();

        ~SquigGobba() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_spitSquigs,
                m_bashinSticks,
                m_cavernousMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcing Spit                      Yes
//

} // namespace GloomspiteGitz
