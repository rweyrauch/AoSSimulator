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

    class Annihilators : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Annihilators(Stormhost stormhost, int numModels, int points);

        ~Annihilators() override = default;

    protected:

    private:

        Weapon  m_hammer{Weapon::Type::Melee, "Meteoric Hammer", 1, 3, 3, 3, -1, 2},
                m_hammerChampion{Weapon::Type::Melee, "Meteoric Hammer", 1, 4, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blazing Imapact                  TODO
// Force of a Falling Star          TODO
//

} // namespace StormcastEternals
