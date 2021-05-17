/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class DeathrattleSkeletons : public SoulblightBase {
    public:

        enum ChampionWeaponOption {
            Ancient_Blade_Or_Spear,
            Champions_Mace_Or_Halberd,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DeathrattleSkeletons(CursedBloodline bloodline, int numModels, ChampionWeaponOption weapon, bool standardBearer, int points);

        ~DeathrattleSkeletons() override;

    private:

        Weapon  m_ancientBladeOrSpear{Weapon::Type::Melee, "Ancient Blade or Spear", 1, 1, 3, 4, 0, 1},
                m_championsMaceOrHalberd{Weapon::Type::Melee, "Champion's Mace or Halberd", 1, 2, 3, 3, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  TODO
// Skeleton Legion                  TODO
//

} //namespace Soulblight
