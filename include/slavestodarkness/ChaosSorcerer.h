/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosSorcerer : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosSorcerer(DamnedLegion legion, MarkOfChaos mark, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ChaosSorcerer() = delete;

        ~ChaosSorcerer() override = default;

    protected:

        void onEndHero(PlayerId player) override;

    private:

        Weapon  m_staff{Weapon::Type::Melee, "Sorcerous Staff", 2, 1, 4, 3, -1, RAND_D3},
                m_blade{Weapon::Type::Melee, "Chaos Runeblade", 1, 2, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 Yes
// Deamonic Power                   Yes
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
