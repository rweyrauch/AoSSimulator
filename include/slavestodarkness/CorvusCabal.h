/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class CorvusCabal : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CorvusCabal(DamnedLegion legion, int numModels);

        CorvusCabal() = delete;

        ~CorvusCabal() override = default;

    protected:

        Rerolls chargeRerolls() const override;

    private:

        Weapon  m_ravenDarts{Weapon::Type::Missile, "Raven Darts", 8, 1, 4, 5, 0, 1},
                m_corvusWeapons{Weapon::Type::Melee, "Corvus Weapons", 1, 1, 4, 4, 0, 1},
                m_corvusWeaponsLeader{Weapon::Type::Melee, "Corvus Weapons (Shadow Piercer)", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Shrike Talon                     Yes
// Death From Above                 Yes
//

} // SlavesToDarkness
