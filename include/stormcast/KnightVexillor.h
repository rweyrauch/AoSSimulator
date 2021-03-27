/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class KnightVexillor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        KnightVexillor();

        ~KnightVexillor() override;

    protected:

        void configure();

        Rerolls iconOfWarChargeReroll(const Unit *unit);

    private:

        Weapon m_warhammer;

        lsignal::slot m_iconOfWarSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon of War                      Yes
// Meteoric Standard                TODO
// Pennant of the Stormbringer      TODO
//

} // namespace StormcastEternals
