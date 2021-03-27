/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class Desolators : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Desolators();

        ~Desolators() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_stormBlast,
                m_thunderAxe,
                m_clawsAndFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fury of the Storm                Yes
// Intolerable Damage               Yes
// Sigmarite Shields                Yes
// Storm Blast                      Yes
//

} // namespace StormcastEternals
