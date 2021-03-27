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

    class LordVeritant : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        LordVeritant();

        ~LordVeritant() override = default;

    protected:

        void configure(PrayersOfTheStormhost prayer);

    private:

        Weapon m_judgementBlade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Faithful Gryph-hound             TODO
// Lantern of Abjuration            Yes
// Sanction                         Yes
// Bound in Service                 TODO
//

} // namespace StormcastEternals
