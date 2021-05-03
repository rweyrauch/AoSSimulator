/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class TheGrymwatch : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TheGrymwatch();

        ~TheGrymwatch() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_weaponsAndClaws,
                m_fangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Royal Retinue                    Yes (see DukeCrackmarrow)
// Quest to Slay the Monster        Yes
//

} // namespace FleshEaterCourt
