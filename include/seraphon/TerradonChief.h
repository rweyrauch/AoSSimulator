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

    class TerradonChief : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TerradonChief(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral);

        TerradonChief() = delete;

        ~TerradonChief() override = default;

    private:

        Weapon  m_skyblade{Weapon::Type::Melee, "Skyblade", 1, 3, 3, 4, -1, 1},
                m_jaws{Weapon::Type::Melee, "Razor-sharp Jaws", 1, 4, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lead from on High                TODO
// Coordinated Attack               TODO
//

} // namespace Seraphon
