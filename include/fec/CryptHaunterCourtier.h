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

    class CryptHaunterCourtier : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CryptHaunterCourtier(GrandCourt court, Delusion delusion, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~CryptHaunterCourtier() override = default;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_massiveBoneClub,
                m_rancidTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Noble Blood                      Yes
// Chosen of the King               Yes
// Muster King's Chosen             Yes
//

} // namespace FleshEaterCourt
