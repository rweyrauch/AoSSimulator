/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class SpiritTorment : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpiritTorment();

        ~SpiritTorment() override = default;

    protected:

        void configure();

    private:

        Weapon m_chains;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Nagash's Bidding                 TODO
// Captured Soul Energy             TODO
//

} // namespace Nighthaunt

