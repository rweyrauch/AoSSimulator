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

    class TheddraSkullscryer : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TheddraSkullscryer(DamnedLegion legion, Lore lore, bool isGeneral);

        TheddraSkullscryer() = delete;

        ~TheddraSkullscryer() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_wand{Weapon::Type::Melee, "Darkoath Wand", 1, 2, 4, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pact of Soul and Iron            Yes
// Enfeeblement                     Yes
//

} // SlavesToDarkness

