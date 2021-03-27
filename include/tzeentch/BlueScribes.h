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

    class TheBlueScribes : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheBlueScribes();

        ~TheBlueScribes() override = default;

    protected:

        void configure(Lore lore);

    private:

        Weapon m_quills,
                m_teethAndHorns;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frantic Scribbling               TODO
// Scrolls of Sorcery               TODO
// Boon of Tzeentch                 Yes
//

} // namespace Tzeentch

