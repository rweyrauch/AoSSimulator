/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {
    class DireWolves : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DireWolves();

        ~DireWolves() override = default;

        bool configure(int numModels);

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_fangsAndClaws,
                m_fangsAndClawsDoom;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Slavering Charge                 Yes
// Vigour Necris                    Yes
//

} //namespace Death
