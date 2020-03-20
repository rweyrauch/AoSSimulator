/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CORVUSCABAL_H
#define CORVUSCABAL_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class CorvusCabal : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CorvusCabal();

        ~CorvusCabal() override = default;

        bool configure(int numModels);

    protected:

        Rerolls chargeRerolls() const override;

        void onWounded() override;

    protected:

        bool m_hasShrikeTalon = true;

    private:

        Weapon m_ravenDarts,
                m_corvusWeapons,
                m_corvusWeaponsLeader;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Shrike Talon                     Yes
// Death From Above                 Yes
//

} // SlavesToDarkness

#endif //CORVUSCABAL_H