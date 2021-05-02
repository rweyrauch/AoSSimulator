/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class RatSwarms : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RatSwarms(int points);

        ~RatSwarms() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_teeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Endless Tide of Rats             TODO
//

} // namespace Skaven
