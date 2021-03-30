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

    class VanariBannerblade : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~VanariBannerblade() override = default;

    protected:

        VanariBannerblade(GreatNation nation, CommandTrait trait, Artefact artefact, bool isGeneral);

    private:

        Weapon m_sword{Weapon::Type::Melee, "Bannerblade's Sword", 1, 4, 2, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// World Banner                     TODO
// Sunmetal Weapons                 TODO
//

} // namespace LuminethRealmLords

