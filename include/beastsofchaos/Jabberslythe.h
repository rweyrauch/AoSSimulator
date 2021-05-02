/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Jabberslythe : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Jabberslythe() = delete;

        ~Jabberslythe() override = default;

        explicit Jabberslythe(Greatfray fray);

    protected:

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_slytheyTongue,
                m_vorpalClaws,
                m_spikedTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Madness                  Yes
// Spurting Bile Blood              Yes
//

} // namespace BeastsOfChaos
