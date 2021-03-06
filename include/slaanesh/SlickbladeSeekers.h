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

    class SlickbladeSeekers : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SlickbladeSeekers(Host host, int numModels, int points);

        ~SlickbladeSeekers() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls chargeRerolls() const override { return Rerolls::Failed; }

    private:

        Weapon m_glaive,
                m_glaiveHunter,
                m_poisonedTongue;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unrivalled Velocity              Yes
// Decapitating Strikes             Yes
//

} // Slannesh
