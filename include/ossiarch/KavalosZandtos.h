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

    class ArchKavalosZandtos : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ArchKavalosZandtos(Legion legion, bool isGeneral);

        ArchKavalosZandtos() = delete;

        ~ArchKavalosZandtos() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_lance{Weapon::Type::Melee, "The Dark Lance", 2, 3, 3, 3, -1, 2},
                m_shield{Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Dark Lance                   Yes
// Hatred of the Living             Yes
// Unstoppable Charge               TODO
// Endless Duty                     TODO
// Still Their Breath               TODO
//

} // namespace OssiarchBonereapers

