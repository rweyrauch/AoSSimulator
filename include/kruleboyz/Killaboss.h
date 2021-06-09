/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kruleboyz/KruleBoyz.h>

namespace Kruleboyz {

    class Killaboss : public Kruleboyz {
    public:

        enum WeaponOption {
            BossHackaAndRustingFlail,
            BossHackaAndSkareshield
        };

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Killaboss(Warclan warclan, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Killaboss() override = default;

    protected:

    private:

        Weapon  m_hacka{Weapon::Type::Melee, "Boss-hacka", 1, 4, 3, 3, -1, 2},
                m_flail{Weapon::Type::Melee, "Rusting Flail", 2, 2, 3, 3, -1, 1},
                m_shiv{Weapon::Type::Melee, "Prived Shiv", 1, 3, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// All Part of Da Plan              TODO
// You Hold 'Em Off                 TODO
//

} // namespace Kruleboyz
