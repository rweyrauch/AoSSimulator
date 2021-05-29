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

    class Dexcessa : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Dexcessa(Host host, bool isGeneral);

        ~Dexcessa() override = default;

    private:

        Weapon m_scourge{Weapon::Type::Melee, "Scourge of Slaanesh", 3, 4, 2, 3, -1, 2},
                m_talons{Weapon::Type::Melee, "Impaling Talons", 1, 2, 3, 3, -2, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fleeting Dance of Death          TODO
// Joyous Battle Fury               TODO
// Mesmerising Lepidoptera          TODO
// Sceptre of Slaanesh              TODO
//

} // Slannesh
