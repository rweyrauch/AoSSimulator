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

    class MortisanSoulmason : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MortisanSoulmason(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        MortisanSoulmason() = delete;

        ~MortisanSoulmason() override = default;

    private:

        Weapon  m_staff{Weapon::Type::Melee, "Soulmason's Staff", 2, 2, 4, 3, -1, RAND_D3},
                m_claws{Weapon::Type::Melee, "Ossified Claws", 1, 2, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mortek Throne                    TODO
// Soul-guide                       Yes
//

} // namespace OssiarchBonereapers

