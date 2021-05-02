/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunefather : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AuricRunefather();

        ~AuricRunefather() override = default;

    protected:

        void configure();

        void onStartHero(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_throwingAxe,
                m_grandAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stare Down                       Yes
// Weapon-breaker                   Yes
// Lodge Leader                     Yes
//

} // namespace Fyreslayers
