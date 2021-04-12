/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class TerradonRiders : public SeraphonBase {
    public:

        enum WeaponOption {
            Starstrike_Javelins,
            Sunleech_Bolas
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        TerradonRiders(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption option);

        TerradonRiders() = delete;

        ~TerradonRiders() override = default;

    private:

        Weapon  m_javelin{Weapon::Type::Missile, "Starstrike Javelin", 12, 2, 4, 3, 0, 1},
                m_javelinLeader{Weapon::Type::Missile, "Starstrike Javelin", 12, 3, 4, 3, 0, 1},
                m_bolas{Weapon::Type::Missile, "Sunleech Bolas", 6, RAND_D6, 4, 3, 0, 1},
                m_bolasLeader{Weapon::Type::Missile, "Sunleech Bolas", 6, RAND_D6, 4, 3, 0, 1},
                m_jaws{Weapon::Type::Melee, "Razor-sharp Jaws", 1, 4, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Cargo                     TODO
//

} // namespace Seraphon
