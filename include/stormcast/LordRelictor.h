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
#include "PrayersOfTheStormhosts.h"

namespace StormcastEternals {

    class LordRelictor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        LordRelictor(Stormhost stormhost, PrayersOfTheStormhost prayer, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordRelictor() override = default;

    protected:

    private:

        Weapon m_relicHammer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Healing Storm                    Yes
// Lightning Storm                  Yes
//

} // namespace StormcastEternals
