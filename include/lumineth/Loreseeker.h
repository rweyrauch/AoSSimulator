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

    class ScinariLoreseeker : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ~ScinariLoreseeker() override = default;

    protected:

        ScinariLoreseeker(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

    private:

        Weapon m_staff{Weapon::Type::Missile, "Eclipsian Staff", 12, 2, 3, 3, -2, RAND_D3},
            m_blade{Weapon::Type::Melee, "Loreseeker Blade", 1, 4, 2, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Loreseeker                       TODO
// Lone Agent                       TODO
//

} // namespace LuminethRealmLords

