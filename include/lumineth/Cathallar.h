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

    class ScinariCathallar : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ScinariCathallar(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~ScinariCathallar() override = default;

    private:

        Weapon m_touch;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Emotional Transferance           TODO
// Darkness of the Soul             TODO
//

} // namespace LuminethRealmLords

