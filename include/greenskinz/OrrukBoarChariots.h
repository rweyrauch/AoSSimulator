/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Greenskinz {

    class OrrukBoarChariots : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OrrukBoarChariots();

        ~OrrukBoarChariots() override = default;

        bool configure(int numModels);

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        void onCharged() override;

    private:

        Weapon m_pigstikkaSpears,
                m_warBoarsTusks;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scythed Wheels                   Yes
// Tusker Charge                    Yes
//

} // namespace Greenskinz
