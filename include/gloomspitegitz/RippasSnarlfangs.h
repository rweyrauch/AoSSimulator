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

    class RippasSnarlfangs : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        RippasSnarlfangs();

        ~RippasSnarlfangs() override = default;

    protected:

        void configure();

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_grotBow,
                m_bossLoppa,
                m_stikka,
                m_bowStave,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Smell Weakness                   Yes
// Ferocious Pounce                 Yes
//

} // namespace GloomspiteGitz
