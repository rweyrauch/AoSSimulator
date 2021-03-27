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

    class DukeCrakmarrow : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DukeCrakmarrow();

        ~DukeCrakmarrow() override = default;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_halberd;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gallant Champion                 Yes
// Muster the Grymwatch             Yes
//

} // namespace FleshEaterCourt
