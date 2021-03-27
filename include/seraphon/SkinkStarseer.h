/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SkinkStarseer : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkinkStarseer();

        ~SkinkStarseer() override = default;

    protected:

        void configure(Lore lore);

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_astralBolt,
                m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cosmic Herald                    Yes
// Astromancer's Staff              TODO
// Control Fate                     Yes
//

} // namespace Seraphon
