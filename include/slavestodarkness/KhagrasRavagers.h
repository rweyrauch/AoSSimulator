/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class KhagrasRavagers : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KhagrasRavagers();

        ~KhagrasRavagers() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon  m_handWeapons{Weapon::Type::Melee, "Chaos Hand Weapons", 1, 3, 3, 3, 0, 1},
                m_mace{Weapon::Type::Melee, "Daemonbound Mace", 1, 3, 3, 3, -2, 1},
                m_staff{Weapon::Type::Melee, "Sorcerer Staff", 3, 1, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chaos Runeshields                Yes
//

} // SlavesToDarkness
