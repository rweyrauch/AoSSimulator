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

    class Sevireth : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~Sevireth() override = default;

    protected:

        Sevireth(bool isGeneral);

    private:

        Weapon m_bow{Weapon::Type::Missile, "Enathrai, the Howling Death", 18, 4, 2, 3, -3, RAND_D3},
            m_bowMelee{Weapon::Type::Melee, "Enathrai, the Howling Death", 3, 2, 2, 3, -2, RAND_D3},
            m_shards{Weapon::Type::Melee, "Swirling Shards", 3, RAND_D3, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Into the Gale                    TODO
// Living Cyclone                   TODO
// Scour                            TODO
// Spirit of the Wind               TODO
// Windmage Symbiosis               TODO
//

} // namespace LuminethRealmLords

