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

    class MortisanBoneshaper : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MortisanBoneshaper(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        MortisanBoneshaper() = delete;

        ~MortisanBoneshaper() override = default;

    private:

        Weapon m_talons{Weapon::Type::Melee, "Ossified Talons", 1, 2, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Boneshaper                       TODO
// Shard-storm                      Yes
//

} // namespace OssiarchBonereapers

