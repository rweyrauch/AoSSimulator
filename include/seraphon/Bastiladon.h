/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Bastiladon : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bastiladon(WayOfTheSeraphon way, Constellation constellation);

        Bastiladon() = delete;

        ~Bastiladon() override = default;

    protected:

        void onWounded() override;

        size_t getDamageTableIndex() const;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_beam{Weapon::Type::Missile, "Solar Engine", 24, 9, 4, 3, -1, 2},
                m_javelins{Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1},
                m_ark{Weapon::Type::Melee, "Ark of Sotek", 3, 18, 4, 6, 0, 1},
                m_tail{Weapon::Type::Melee, "Bludgeoning Tail", 2, 3, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Light of the Heavens             Yes
// Tide of Snakes                   Yes
//

} // namespace Seraphon
