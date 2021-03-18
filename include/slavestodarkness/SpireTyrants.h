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

    class SpireTyrants : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpireTyrants();

        ~SpireTyrants() override = default;

        bool configure(int numModels);


    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_gladiatorialWeapons,
                m_gladiatorialWeaponsChampion,
                m_gladiatorialWeaponsHeadclaimer,
                m_gladiatorialWeaponsDestroyer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pit Fighters                     Yes
//

} // SlavesToDarkness

