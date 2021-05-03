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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlissbarbArchers(Host host, int numModels, int points);

        ~BlissbarbArchers() override = default;

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
