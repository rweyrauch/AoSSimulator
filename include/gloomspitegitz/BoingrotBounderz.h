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

    class BoingrotBounderz : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BoingrotBounderz(int points);

        ~BoingrotBounderz() override = default;

        bool configure(int numModels);

    protected:

        void onCharged() override;

        int toWoundModifier(const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_fangFilledGob,
                m_pokinLance,
                m_pokinLanceBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Boing! Smash!                    Yes
// Lances of the Bounderz           Yes
//

} // namespace GloomspiteGitz
