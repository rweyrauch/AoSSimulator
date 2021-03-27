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

    class MortisanSoulreaper : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        MortisanSoulreaper();

        ~MortisanSoulreaper() override = default;

    protected:

        void configure(Lore lore);

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_scythe;
        Lore m_lore = Lore::Empower_Nadirite_Weapons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathly Touch                    Yes
// Soulreaper                       Yes
// Soul-blast                       TODO
//

} // namespace OssiarchBonereapers

