/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class BjorgenThundrik : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BjorgenThundrik();

        ~BjorgenThundrik() override;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

        int atmosphericIsolation(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_anatomiser,
                m_instruments;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aetheric Augmentation            Yes
// Atmospheric Isolation            Yes
//

} // namespace KharadronOverlords

