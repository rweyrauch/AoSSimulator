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

        ScionsOfTheFlame(DamnedLegion legion, int numModels);

        ScionsOfTheFlame() = delete;

        ~ScionsOfTheFlame() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_pots{Weapon::Type::Missile, "Flameburst Pots", 8, 1, 4, 3, 0, 1},
                m_scionWeapons{Weapon::Type::Melee, "Scion Weapons", 1, 1, 4, 4, 0, 1},
                m_scionWeaponsLeaders{Weapon::Type::Melee, "Scion Weapons", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Inferno Priest                   Yes
// All Shall Burn                   Yes
//

} // SlavesToDarkness
