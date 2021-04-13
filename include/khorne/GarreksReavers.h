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

    class GarreksReavers : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~GarreksReavers() override = default;

    protected:

        GarreksReavers();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon  m_garreksAxe{Weapon::Type::Melee, "Garrek's Blooddrinker Axe", 1, 3, 3, 4, -1, 1},
                m_karusAxe{Weapon::Type::Melee, "Karsus' Chained Axe", 2, RAND_D3, 4, 4, 0, 1},
                m_saeksAxe{Weapon::Type::Melee, "Saek's Meatripper Axe", 1, 1, 3, 4, -1, 1},
                m_blades{Weapon::Type::Melee, "Reaver Blades", 1, 1, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gorebeard                        Yes
// Frenzied Devotion                Yes
// Reaver Blades                    Yes
// Grisly Trophies                  Yes
//

} // namespace Khorne
