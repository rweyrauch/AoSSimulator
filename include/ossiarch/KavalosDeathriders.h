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

    class KavalosDeathriders : public OssiarchBonereaperBase {
    public:

        enum WeaponOption {
            Nadirite_Blade_And_Shield,
            Nadirate_Spear_And_Shield
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        KavalosDeathriders(Legion legion, int numModels, WeaponOption option, bool necrophoros);

        KavalosDeathriders() = delete;

        ~KavalosDeathriders() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int runModifier() const override;

        int chargeModifier() const override;

    private:

        bool m_necrophoros = false;

        Weapon  m_blade{Weapon::Type::Melee, "Nadirite Blade", 1, 3, 3, 4, -1, 1},
                m_spear{Weapon::Type::Melee, "Nadirite Spear", 2, 3, 3, 4, 0, 1},
                m_bladeHekatos{Weapon::Type::Melee, "Nadirite Blade", 1, 4, 3, 4, -1, 1},
                m_spearHekatos{Weapon::Type::Melee, "Nadirite Spear", 2, 4, 3, 4, 0, 1},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Necrophoros                      Yes
// Nadirite Weapons                 Yes
// Deathrider Wedge                 TODO
//

} // namespace OssiarchBonereapers

