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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KorghosKhul() = delete;

        ~KorghosKhul() override = default;

    protected:

        explicit KorghosKhul(bool isGeneral);

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onEndCombat(PlayerId player) override;

        void onEnemyModelWoundedWithWeapon(Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon  m_axeOfKhorne{Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3},
                m_clawsAndFangs{Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 4, -1, 1};

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
