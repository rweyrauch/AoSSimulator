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

    class MorghastHarbingers : public OssiarchBonereaperBase {
    public:

        enum WeaponOptions {
            Spirit_Halberd,
            Spirit_Swords,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MorghastHarbingers(Legion legion, int numModels, WeaponOptions weapons, int points);

        MorghastHarbingers() = delete;

        ~MorghastHarbingers() override = default;

    private:

        WeaponOptions m_weaponOption = Spirit_Halberd;

        Weapon  m_spiritHalberd{Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3},
                m_spiritSwords{Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Heralds of the Accursed One      TODO
// Harbinger of Death               TODO
//

} //namespace OssiarchBonereapers

