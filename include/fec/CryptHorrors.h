/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class CryptHorrors : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CryptHorrors(GrandCourt court, Delusion delusion, int numModels, int points);

        ~CryptHorrors() override = default;

    protected:

        void onStartHero(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_clubsAndTalons,
                m_clubsAndTalonsHaunter;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Warrior Elite                    Yes
// Noble Blood                      Yes
// Chosen of the King               Yes
//

} // namespace FleshEaterCourt
