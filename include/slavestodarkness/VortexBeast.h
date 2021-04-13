/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class MutalithVortexBeast : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MutalithVortexBeast(DamnedLegion legion, MarkOfChaos mark);

        MutalithVortexBeast() = delete;

        ~MutalithVortexBeast() override = default;

    protected:

        void onStartHero(PlayerId player) override;

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon  m_claws{Weapon::Type::Melee, "Crushing Claws", 2, 4, 4, 1, -1, RAND_D3},
                m_maw{Weapon::Type::Melee, "Betentacled Maw", 2, RAND_3D6, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mutant Regeneration              Yes
// Aura of Mutation                 Yes
//    Hideous Disfigurement         Yes
//    Troggbrains                   Yes
//    Gift of Mutations             Yes
//    Tide of Transmogrification    Yes
//    Maelstrom of Change           Yes
//    Spawnchange                   Yes
//

} // SlavesToDarkness

