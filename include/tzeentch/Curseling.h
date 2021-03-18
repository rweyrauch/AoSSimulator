/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class CurselingEyeOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CurselingEyeOfTzeentch();

        ~CurselingEyeOfTzeentch() override = default;

        bool configure();

    protected:

    private:

        Weapon m_sword,
                m_flail,
                m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disrupter of the Arcane          TODO
// Vessel of Chaos                  TODO
// Glean Magic                      TODO
//

} // namespace Tzeentch

