/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class MournfangPack : public MawtribesBase {
    public:

        enum WeaponOption {
            Culling_Club_Or_Prey_Hacker_And_Ironfist,
            Gargant_Hacker
        };

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static void Init();

        MournfangPack() = delete;

        ~MournfangPack() override = default;

    protected:

        MournfangPack(Mawtribe tribe, int numModels, WeaponOption weaponOption, bool hornblower, bool bannerBearer);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

    private:

        WeaponOption m_option = Culling_Club_Or_Prey_Hacker_And_Ironfist;

        Weapon m_pistol{Weapon::Type::Missile, "Ironlock Pistol", 12, 1, 4, 3, -1, RAND_D3},
                m_clubOrHacker{Weapon::Type::Melee, "Culling Club or Prey Hacker", 1, 3, 3, 3, 0, 2},
                m_gargantHacker{Weapon::Type::Melee, "Gargant Hacker", 2, 2, 4, 3, -1, 3},
                m_tusks{Weapon::Type::Melee, "Tusks", 1, 4, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Banner Bearer                    Yes
// Ironfist                         Yes
// Mournfang Charge                 Yes
//

} // namespace OgorMawtribes

