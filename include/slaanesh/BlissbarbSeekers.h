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

    class BlissbarbSeekers : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlissbarbSeekers(Host host, int numModels, int points);

        ~BlissbarbSeekers() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_bow,
                m_bowTempter,
                m_blade,
                m_poisonedTongue;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Flawless Accuracy                Yes
// Vectors of Agony                 Yes
//

} // Slannesh
