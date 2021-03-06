/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz {

    class OrrukBrutes : public Ironjawz {
    public:

        enum WeaponOption {
            Two_Brute_Choppas = 0,
            Jagged_Gorehacka,
        };
        enum BossWeaponOption {
            Boss_Choppa,
            Boss_Klaw,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        OrrukBrutes(Warclan warclan, int numModels, WeaponOption weapons, int numGoreChoppas, BossWeaponOption bossWeapon, int points);

        ~OrrukBrutes() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    protected:

        Weapon m_twoBruteChoppas,
                m_gorehacka,
                m_gorechoppa,
                m_bossChoppa,
                m_bossKlawAndBruteSmasha;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Duff Up da Big Thing             Yes
//

} // namespace Ironjawz
