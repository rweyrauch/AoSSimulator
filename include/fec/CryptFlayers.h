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

    class CryptFlayers : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CryptFlayers(GrandCourt court, Delusion delusion, int numModels, int points);

        ~CryptFlayers() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_deathScream,
                m_piercingTalons,
                m_piercingTalonsInfernal;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Skewering Strike                 Yes
// Death Scream                     Yes
//

} // namespace FleshEaterCourt
