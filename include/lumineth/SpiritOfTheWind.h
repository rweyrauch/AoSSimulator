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

    class HurakanSpiritOfTheWind : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~HurakanSpiritOfTheWind() override = default;

    protected:

        HurakanSpiritOfTheWind(GreatNation nation);

    private:

        Weapon m_bow{Weapon::Type::Missile, "Bow of the Wind's Vengeance", 18, 4, 2, 3, -2, RAND_D3},
            m_bowMelee{Weapon::Type::Melee, "Bow of the Wind's Vengeance", 3, 2, 2, 3, -2, RAND_D3},
            m_shards{Weapon::Type::Melee, "Swirling Shards", 3, RAND_D3, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Into the Gale                    TODO
// Living Cyclone                   TODO
// Spirit of the Wind               TODO
// Windmage Symbiosis               TODO
//

} // namespace LuminethRealmLords

