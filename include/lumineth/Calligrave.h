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

    class ScinariCalligrave : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ~ScinariCalligrave() override = default;

    protected:

        ScinariCalligrave(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

    private:

        Weapon m_blade{Weapon::Type::Melee, "Calligrave Blade", 3, RAND_D3, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Realmscribe                      TODO
// Erasure                          TODO
//

} // namespace LuminethRealmLords

