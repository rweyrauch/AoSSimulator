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

    class GodswornHunt : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit GodswornHunt(DamnedLegion legion);

        GodswornHunt() = delete;

        ~GodswornHunt() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_huntingBow{Weapon::Type::Missile, "Hunting Bow", 24, 2, 4, 4, 0, 1},
                m_javelin{Weapon::Type::Missile, "Ensorcelled Javelin", 12, 1, 3, 3, -1, RAND_D3},
                m_knife{Weapon::Type::Melee, "Darkoath Knife", 1, 3, 4, 4, 0, 1},
                m_greatWeapon{Weapon::Type::Melee, "Great Weapon", 1, 2, 4, 3, -1, 2},
                m_bowMelee{Weapon::Type::Melee, "Hunting Bow", 1, 1, 4, 5, 0, 1},
                m_bite{Weapon::Type::Melee, "Savage Bite", 1, 2, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pact of Soul and Iron            Yes
//

} // SlavesToDarkness

