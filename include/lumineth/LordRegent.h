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

    class VanariLordRegent : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ~VanariLordRegent() override = default;

    protected:

        VanariLordRegent(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

    private:

        Weapon m_sword{Weapon::Type::Melee, "Regent's Sword", 1, 5, 2, 3, -1, 1},
            m_hornsAndClaws{Weapon::Type::Melee, "Horns and Claws", 1, 3, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Purest Aetherquartz              TODO
// Sunmetal Weapons                 TODO
// Greater Power of Hysh            TODO
//

} // namespace LuminethRealmLords

