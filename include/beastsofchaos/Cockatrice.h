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

    class Cockatrice : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Cockatrice() = delete;

        ~Cockatrice() override = default;

        explicit Cockatrice(Greatfray fray);

    protected:

        void onStartShooting(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_petrifyingGaze,
                m_viciousBeak,
                m_swordlikeTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Petrifying Gaze                  Yes
// Maddened Ferocity                Yes
//

} // namespace BeastsOfChaos
