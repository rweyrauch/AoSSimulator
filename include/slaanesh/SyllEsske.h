/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class SyllEsske : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SyllEsske(Host host, Lore lore, bool isGeneral);

        ~SyllEsske() override = default;

    private:

        Weapon m_axeOfDominion,
                m_scourgingWhip;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Symbiosis                 TODO
// Lithe and Swift                  Yes
// Subvert                          Yes
// Regal Authority                  Yes
//

} // Slannesh
