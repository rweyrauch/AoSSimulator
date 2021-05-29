/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class AwakenedWyldwood : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AwakenedWyldwood();

        ~AwakenedWyldwood() override = default;

    protected:

    private:

        Weapon m_placeholder{Weapon::Type::Melee, "Placeholder", 1, 0, 1, 1, 0, 0};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Overgrown Wilderness             TODO
// Roused by Magic                  TODO
// Wyldwood                         TODO
//

} // namespace Sylvaneth
