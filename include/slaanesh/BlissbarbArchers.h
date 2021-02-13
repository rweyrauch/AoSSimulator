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

    class BlissbarbArchers : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BlissbarbArchers();

        ~BlissbarbArchers() override = default;

        bool configure(int numModels);

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_bow,
                m_bowTempter,
                m_blade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Homonculus                       Yes
// Light-footed Killers             Yes
//

} // Slannesh
