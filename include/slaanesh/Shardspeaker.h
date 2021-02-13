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

    class ShardspeakerOfSlaanesh : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ShardspeakerOfSlaanesh();

        ~ShardspeakerOfSlaanesh() override = default;

        bool configure(Lore lore);

    protected:


    private:

        Weapon m_staff,
            m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mist Lurkers                     TODO
// Twisted Mirror                   TODO
// Reflection Eternal               TODO
//

} // Slannesh
