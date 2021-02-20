/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class VortemisTheAllSeeing : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VortemisTheAllSeeing();

        ~VortemisTheAllSeeing() override = default;

        bool configure(Lore lore);

    protected:

    private:

        Weapon m_staffMissile,
                m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Magic-touched                    TODO
// Sorcerous Insight                TODO
//

} // namespace Tzeentch

