/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Furies : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Furies(DamnedLegion legion, int numModels, int points);

        Furies() = delete;

        ~Furies() override = default;

    private:

        Weapon m_daggerAndClaws{Weapon::Type::Melee, "Razor-sharp Dagger and Claws", 1, 2, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sneaky Little Devils             TODO
//

} // SlavesToDarkness
