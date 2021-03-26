/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Magmadroth.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunefatherOnMagmadroth : public Magmadroth {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunefatherOnMagmadroth();

        ~AuricRunefatherOnMagmadroth() override = default;

    protected:

        void configure(MountTrait trait);

        void onEndCombat(PlayerId player) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_throwingAxe,
                m_grandAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Stare Down                       Yes
// Weapon-breaker                   Yes
// Steadfast Advance                TODO
//

} // namespace Fyreslayers
