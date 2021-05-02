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

    class UntamedBeasts : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        UntamedBeasts(DamnedLegion legion, int numModels, int points);

        UntamedBeasts() = delete;

        ~UntamedBeasts() override = default;

    protected:

    private:

        Weapon  m_harpoonFirstFang{Weapon::Type::Missile, "Jagged Harpoon", 8, 1, 4, 3, -1, 2},
                m_huntingWeapons{Weapon::Type::Melee, "Hunting Weapons", 1, 1, 4, 4, 0, 1},
                m_huntingWeaponsHeartEater{Weapon::Type::Melee, "Hunting Weapons (Heart-eater)", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unleash the Beast                Partial/TODO
//

} // SlavesToDarkness
