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

    class SkullCannons : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkullCannons() = delete;

        ~SkullCannons() override = default;

    protected:

        SkullCannons(SlaughterHost host, int numModels, int points);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        void onStartCombat(PlayerId player) override;

        void onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) override;

    private:

        bool m_hasFoughtTwice = false;

        Weapon  m_burningSkulls{Weapon::Type::Missile, "Burning Skulls", 30, 1, 3, 3, -2, RAND_D6},
                m_hellblades{Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1},
                m_gnashingMaw{Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Burning Skulls                   Yes
// Grind their Bones, Seize their Skulls    Yes
// Decapitating Blow                Yes
//

} // namespace Khorne
