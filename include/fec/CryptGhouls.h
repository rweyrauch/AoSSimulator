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

    class CryptGhouls : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CryptGhouls(GrandCourt court, Delusion delusion, int numModels, int points);

        ~CryptGhouls() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_teethAndClaws,
                m_teethAndClawsGhast;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Boundless Ferocity               Yes
// Royal Approval                   Yes
//

} // namespace FleshEaterCourt
