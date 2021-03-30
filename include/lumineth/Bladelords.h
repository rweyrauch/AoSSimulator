/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class VanariBladelords : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~VanariBladelords() override = default;

    protected:

        VanariBladelords(int numModels, bool seneschalDualBlades, GreatNation nation);

    private:

        Weapon m_greatbladeStrike{Weapon::Type::Melee, "Sunmetal Greatblade: Perfect Strike", 2, 1, 0, 2, -2, 1},
               m_greatbladeBlows{Weapon::Type::Melee, "Sunmetal Greatblade: Flurry of Blows", 2, 1, 3, 3, 0, 1},
               m_greatbladeSeneschalStrike{Weapon::Type::Melee, "Sunmetal Greatblade: Perfect Strike", 2, 2, 0, 2, -2, 1},
               m_greatbladeSeneschalBlow{Weapon::Type::Melee, "Sunmetal Greatblade: Flurry of Blows", 2, 1, 3, 3, 0, 1},
               m_dualBlades{Weapon::Type::Melee, "Sunmetal Dual Blades", 1, 3, 2, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------

} // namespace LuminethRealmLords

