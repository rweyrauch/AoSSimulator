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

    class Praetors : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Praetors(Stormhost stormhost, int numModels, int points);

        ~Praetors() override = default;

    protected:

    private:

        Weapon  m_halberd{Weapon::Type::Melee, "Soulguard's Halberd", 1, 3, 3, 3, -1, 2},
                m_halberdChampion{Weapon::Type::Melee, "Soulguard's Halberd", 1, 4, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Soul-forged Guardians            TODO
//

} // namespace StormcastEternals
