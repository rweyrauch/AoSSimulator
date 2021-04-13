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

    class Ghorgon : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Ghorgon() = delete;

        ~Ghorgon() override = default;

        explicit Ghorgon(Greatfray fray);

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_butcheringBlades,
                m_hugeSlaveringMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ravenous Bloodgreed              Yes
// Swallow Whole                    Yes
//

} // namespace BeastsOfChaos
