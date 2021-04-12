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

    class LiegeKavalos : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        LiegeKavalos(Legion legion, CommandTrait trait, Artefact artefact, bool isGeneral, Lore lore = Lore::None);

        LiegeKavalos() = delete;

        ~LiegeKavalos() override = default;

    private:

        Weapon  m_blade{Weapon::Type::Melee, "Commander's Blade", 1, 3, 3, 3, -1, 2},
                m_shield{Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Charge               TODO
// Endless Duty                     TODO
//

} // namespace OssiarchBonereapers

