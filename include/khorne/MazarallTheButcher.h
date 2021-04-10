/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class MazarallTheButcher : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MazarallTheButcher(SlaughterHost host, bool isGeneral);

        MazarallTheButcher() = delete;

        ~MazarallTheButcher() override = default;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon  m_shieldWrath{Weapon::Type::Missile, "Ancyte Shield's Wrath", 12, RAND_D6, 5, 3, -1, 1},
                m_harrowMeat{Weapon::Type::Melee, "Harrow Meat", 2, 4, 3, 3, -2, 3},
                m_shieldBlades{Weapon::Type::Melee, "Ancyte Shield's Blades", 1, 5, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bloody Charge                    Yes
// Harrow Meat's Hunger             Yes
// The Ancyte Shield                Yes
// The Butcher's Due                Yes
//

} // namespace Khorne
