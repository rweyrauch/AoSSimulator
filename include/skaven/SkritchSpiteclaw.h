/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class SkritchSpiteclaw : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkritchSpiteclaw();

        ~SkritchSpiteclaw() override = default;

    protected:

        void configure();

    private:

        Weapon m_halberd;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// There are Always More            TODO
// Gnash-gnaw on the Bones!         TODO
//

} // namespace Skaven
