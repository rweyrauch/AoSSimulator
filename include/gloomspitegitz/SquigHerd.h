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

    class SquiqHerd : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SquiqHerd(int numModels, int points);

        ~SquiqHerd() override = default;

    protected:

        bool hasHerder() const;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

        void onFlee(int numFled) override;

    private:

        Weapon m_fangFilledGob,
                m_squigProdder;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Go Dat Way!                      Yes
// Squigs Go Wild                   Yes
//

} // namespace GloomspiteGitz
