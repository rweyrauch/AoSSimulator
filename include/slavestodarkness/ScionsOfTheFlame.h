/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ScionsOfTheFlame : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ScionsOfTheFlame();

        ~ScionsOfTheFlame() override = default;

        bool configure(int numModels);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_pots,
                m_scionWeapons,
                m_scionWeaponsLeaders;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Inferno Priest                   Yes
// All Shall Burn                   Yes
//

} // SlavesToDarkness
