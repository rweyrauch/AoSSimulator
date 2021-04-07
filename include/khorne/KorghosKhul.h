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

    class KorghosKhul : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KorghosKhul();

        ~KorghosKhul() override = default;

    protected:

        void configure();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onEndCombat(PlayerId player) override;

        void onEnemyModelWoundedWithWeapon(Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon m_axeOfKhorne,
                m_clawsAndFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Favoured of Khorne               Yes
// Aqshy's Bane                     Yes
// Collar of Khorne                 Yes
// Reality-splitting Axe            Yes
// Lord of the Goretide             Yes
//

} // namespace Khorne
