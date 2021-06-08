/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class SneakySnufflers : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SneakySnufflers(Allegiance allegiance, int numModels, int points);

        ~SneakySnufflers() override = default;

    protected:

    private:

        Weapon m_sickle,
                m_gnashers;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Looncap Mushrooms                TODO
//

} // namespace GloomspiteGitz
