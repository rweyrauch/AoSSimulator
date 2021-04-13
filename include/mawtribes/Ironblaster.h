/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Ironblaster : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Ironblaster() = delete;

        ~Ironblaster() override = default;

    protected:

        Ironblaster(Mawtribe tribe);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_cannonBall{Weapon::Type::Missile, "Ironblaster Cannon: Cannon Ball", 24, 1, 4, 2, -2, RAND_D6},
                m_hailShot{Weapon::Type::Missile, "Ironblaster Cannon: Hail Shot", 12, 6, 3, 3, -1, 1},
                m_clubber{Weapon::Type::Melee, "Gunner's Clubber", 1, 3, 3, 3, 0, 2},
                m_horns{Weapon::Type::Melee, "Rhinox's Sharp Horns", 1, 2, 4, 3, -1, RAND_D3},
                m_blade{Weapon::Type::Melee, "Scrapper's Jagged Blade", 1, 2, 5, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lethal Payload                   Yes
// Rhinox Charge                    Yes
//

} // namespace OgorMawtribes
