/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class KnightArcanum : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        KnightArcanum(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~KnightArcanum() override = default;

    protected:

    private:

        Weapon m_stave;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------

} // namespace StormcastEternals
