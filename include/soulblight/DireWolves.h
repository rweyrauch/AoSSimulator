/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {
    class DireWolves : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DireWolves(CursedBloodline bloodline, int numModels, int points);

        ~DireWolves() override = default;

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_fangsAndClaws,
                m_fangsAndClawsDoom;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Slavering Charge                 Yes
//

} //namespace Soulblight
