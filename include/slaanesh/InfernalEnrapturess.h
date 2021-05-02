/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class InfernalEnrapturess : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        InfernalEnrapturess();

        ~InfernalEnrapturess() override = default;

    protected:

        void configure();

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_ravagingClaw,
                m_lyreCacophonousMelody,
                m_lyreEuphonicBlast;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Discordant Disruption            TODO
// Versatile Instrument             Yes
//

} // Slannesh
