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

    class Salamanders : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Salamanders();

        ~Salamanders() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_streamOfFire,
                m_jaws,
                m_goad;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// It Burns!                        TODO
//

} // namespace Seraphon
