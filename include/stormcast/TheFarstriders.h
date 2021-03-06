/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace StormcastEternals {

    class TheFarstriders : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TheFarstriders();

        ~TheFarstriders() override = default;

    protected:

    private:

        Weapon m_boltstormPistol,
                m_shockHandaxe,
                m_stormSaber;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Astral Compass                   TODO
// Star Falcon                      TODO
// Tireless Hunters                 Yes
//

} // namespace StormcastEternals
