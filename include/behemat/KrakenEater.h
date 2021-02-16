/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class KrakenEater : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KrakenEater();

        ~KrakenEater() override = default;

        bool configure();

    private:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int terror(const Unit *unit);

        Weapon m_debris,
            m_stomp,
            m_grip,
            m_warclub;

        lsignal::slot m_connection;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Almighty Stomp                   Yes
// Crushing Charge                  Yes
// Death Grip                       Yes
// Longshanks                       TODO
// Get Orf Me Land!                 TODO
// Stuff 'em in Me Net              TODO
// Son of Behemat                   TODO
// Terror                           Yes
// Timberrrrr!                      TODO
//

} // namespace SonsOfBehemat
