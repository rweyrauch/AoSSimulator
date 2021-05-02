/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class ImmortisGuard : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ImmortisGuard(Legion legion, int numModels, int points);

        ImmortisGuard() = delete;

        ~ImmortisGuard() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_halberd{Weapon::Type::Melee, "Dread Halberd", 2, 2, 3, 3, -2, 2},
                m_battleShield{Weapon::Type::Melee, "Battle Shield", 1, 2, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Combination               Yes
// Soulbound Protectors             TODO
// Crushing Assault                 TODO
//

} // namespace OssiarchBonereapers

