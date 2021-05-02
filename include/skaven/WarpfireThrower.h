/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class WarpfireThrower : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WarpfireThrower();

        ~WarpfireThrower() override = default;

    protected:

        void configure();

        int generateMortalWounds(const Unit *unit) override;

    private:

        Weapon m_warpfireThrower,
                m_rustyKnives;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Warpfire                         Yes
// More-more Warpfire!              Yes
//

} // namespace Skaven
