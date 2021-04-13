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

    class CryptInfernalCourtier : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CryptInfernalCourtier();

        ~CryptInfernalCourtier() override = default;

    protected:

        void configure();

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId playerId) override;

    private:

        Weapon m_foetidBreath,
                m_skeweringTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Skewering Strike                 Yes
// Muster Royal Guard               Yes
//

} // namespace FleshEaterCourt
