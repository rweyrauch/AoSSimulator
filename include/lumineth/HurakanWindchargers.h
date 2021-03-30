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

    class HurakanWindchargers : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~HurakanWindchargers() override = default;

    protected:

        HurakanWindchargers(int numModels, GreatNation nation);

    private:

        Weapon m_bow{Weapon::Type::Missile, "Windcharger Bow", 12, 2, 3, 3, -1, 1},
                m_bowSeneschal{Weapon::Type::Missile, "Windcharger Bow", 12, 3, 3, 3, -1, 1},
                m_bowMelee{Weapon::Type::Melee, "Windcharger Bow", 3, 1, 3, 3, -2, 1},
                m_bowSeneschalMelee{Weapon::Type::Melee, "Windcharger Bow", 3, 2, 3, 3, -2, 1},
                m_claws{Weapon::Type::Melee, "Claws", 1, 2, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Windcharger Arrows               TODO
// Go Where the Wind Blows          TODO
// Standard Bearer                  TODO
//

} // namespace LuminethRealmLords

