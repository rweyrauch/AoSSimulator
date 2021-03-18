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

    class OrrukGreatShaman : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OrrukGreatShaman();

        ~OrrukGreatShaman() override = default;

        bool configure(bool warboar);

    protected:

        int castingModifier() const override;

    private:

        Weapon m_totemicStaff,
                m_boarsTusks;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Waaagh! Energy                   Yes
// Gaze of Mork                     TODO

} // namespace Greenskinz
