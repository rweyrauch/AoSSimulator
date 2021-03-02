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

    class CryptGhastCourtier : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CryptGhastCourtier();

        ~CryptGhastCourtier() override = default;

        bool configure();

    protected:

        void onStartHero(PlayerId playerId) override;

    private:

        Weapon m_boneClub,
                m_filthyClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trophy Hunter                    TODO
// Muster Serfs                     Yes
//

} // namespace FleshEaterCourt
