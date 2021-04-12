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

    class Vokmortian : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Vokmortian(Legion legion, Lore lore, bool isGeneral);

        Vokmortian() = delete;

        ~Vokmortian() override;

    protected:

        int grimWarning(const Unit *unit);

    private:

        Weapon  m_gazeOfDeath{Weapon::Type::Missile, "Gaze of Death", 12, 1, 3, 2, -1, RAND_D3},
                m_staff{Weapon::Type::Melee, "Staff of Retribution", 2, 2, 3, 3, -1, RAND_D3};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Contract of Nagash               TODO
// Grim Warning                     Partial/TODO
// Mortal Touch                     Yes
//

} // namespace OssiarchBonereapers

