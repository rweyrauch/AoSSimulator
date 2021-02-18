/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class BloodthirsterOfInsensateRage : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodthirsterOfInsensateRage();

        ~BloodthirsterOfInsensateRage() override = default;

        bool configure();

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onRestore() override;

    private:

        Weapon m_greatAxeOfKhorne;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rage Unbound                     Yes
// Outrageous Carnage               Yes
// Bloodthirsty Charge              Yes
//

} // namespace Khorne
