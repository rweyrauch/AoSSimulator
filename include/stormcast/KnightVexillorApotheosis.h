/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class KnightVexillorApotheosis : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        KnightVexillorApotheosis(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~KnightVexillorApotheosis() = default;

    protected:

    private:

        Weapon m_warblade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Banner of the Reforged       TODO
// Soul-charged Icon                TODO
//

} // namespace StormcastEternals
