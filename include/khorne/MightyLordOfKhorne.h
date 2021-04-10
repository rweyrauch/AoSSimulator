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

        MightyLordOfKhorne() = delete;

        ~MightyLordOfKhorne() override = default;

    protected:

        MightyLordOfKhorne(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        void onEnemyModelWoundedWithWeapon(Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon  m_axeOfKhorne{Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3},
                m_bloodDarkClaws{Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1};

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
