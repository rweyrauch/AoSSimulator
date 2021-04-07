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

    class MightyLordOfKhorne : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MightyLordOfKhorne();

        ~MightyLordOfKhorne() override = default;

    protected:

        void configure();

        void onEnemyModelWoundedWithWeapon(Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon m_axeOfKhorne,
                m_bloodDarkClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Collar of Khorne                 Yes
// Reality-splitting Axe            Yes
// Gorelord                         Yes
//

} // namespace Khorne
