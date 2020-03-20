/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SQUIGHERD_H
#define SQUIGHERD_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class SquiqHerd : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        SquiqHerd();

        ~SquiqHerd() override = default;

        bool configure(int numModels);

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

#endif //SQUIGHERD_H