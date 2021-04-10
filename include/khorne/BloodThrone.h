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

    class HeraldOfKhorneOnBloodThrone : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        HeraldOfKhorneOnBloodThrone() = delete;

        ~HeraldOfKhorneOnBloodThrone() override = default;

    protected:

        HeraldOfKhorneOnBloodThrone(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEnemyModelWoundedWithWeapon(Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon  m_bladeOfBlood{Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1},
                m_hellblades{Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1},
                m_gnashingMaw{Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Blood Throne                 Yes
// Gorefeast                        Yes
// Decapitating Blow                Yes
//

} // namespace Khorne
