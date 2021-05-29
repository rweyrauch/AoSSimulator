/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class Synessa : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Synessa(Host host, Lore lore, bool isGeneral);

        ~Synessa() override = default;

    private:

        Weapon  m_staff{Weapon::Type::Missile, "Staff of Slaanesh", 18, 1, 0, 0, 0, 0},
                m_talons{Weapon::Type::Melee, "Impaling Talons", 1, 3, 3, 3, -2, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mesmerising Lepidoptera          TODO
// Staff of Slaanesh                TODO
// The Voice of Slaanesh            TODO
// Whispers of Doubt                TODO
//

} // Slannesh
