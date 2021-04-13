/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class BrokkGrungsson : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BrokkGrungsson();

        ~BrokkGrungsson() override = default;

    protected:

        void configure();

        void onCharged() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_boast,
                m_charter,
                m_aetherblasters,
                m_saw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Custom-built Dirigible Suit      Yes
// Endrinharness                    Yes
// Hitcher                          TODO
// First Rule of Grungsson          TODO
//

} // namespace KharadronOverlords

