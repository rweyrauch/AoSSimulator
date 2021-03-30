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

    class HurakanWindmage : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~HurakanWindmage() override = default;

    protected:

        HurakanWindmage(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

    private:

        Weapon m_aspiragillum{Weapon::Type::Melee, "Aspiragillum", 3, 2, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fan of Redirection               TODO
// Windleap                         TODO
// Windblast Vortex                 TODO
//

} // namespace LuminethRealmLords

