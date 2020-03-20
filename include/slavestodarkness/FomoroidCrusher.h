/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FOMOROIDCRUSHER_H
#define FOMOROIDCRUSHER_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class FomoroidCrusher : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        FomoroidCrusher();

        ~FomoroidCrusher() override = default;

        bool configure();

    protected:

        void onCharged() override;

        int rollChargeDistance() const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_hurledTerrain,
                m_fists;
        mutable int m_lastChargeDistance = 0;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rampage                          Yes
// Insurmountable Strength          TODO
//

} // SlavesToDarkness

#endif //FOMOROIDCRUSHER_H
