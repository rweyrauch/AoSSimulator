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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkinkStarseer(WayOfTheSeraphon way, Constellation constellation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        SkinkStarseer() = delete;

        ~SkinkStarseer() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_astralBolt{Weapon::Type::Missile, "Astral Bolt", 18, 2, 3, 3, -1, RAND_D3},
                m_staff{Weapon::Type::Melee, "Astromancer's Staff", 2, 2, 4, 3, -1, RAND_D3};

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
