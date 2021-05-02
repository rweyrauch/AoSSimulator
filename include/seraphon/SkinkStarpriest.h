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

    class SkinkStarpriest : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkinkStarpriest(WayOfTheSeraphon way, Constellation constellation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        SkinkStarpriest() = delete;

        ~SkinkStarpriest() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_venombolt{Weapon::Type::Missile, "Venombolt", 18, 2, 3, 3, -1, 1},
                m_staff{Weapon::Type::Melee, "Serpent Staff", 1, 2, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Astral Herald                    Yes
// Serpent Staff                    Yes
// Blazing Starlight                Yes
//

} // namespace Seraphon
