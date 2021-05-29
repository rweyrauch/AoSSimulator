/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Jabberslythe : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Jabberslythe() = delete;

        ~Jabberslythe() override = default;

        explicit Jabberslythe(Greatfray fray);

    protected:

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        Weapon  m_slytheyTongue{Weapon::Type::Missile, "Slythey Tongue", 9, 3, 3, 3, -1, 1},
                m_vorpalClaws{Weapon::Type::Melee, "Vorpal Claws", 1, 6, 3, 3, -2, 1},
                m_spikedTail{Weapon::Type::Melee, "Spiked Tail", 3, 1, 4, 2, -2, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Madness                  TODO
// Spurting Bile Blood              Yes
//

} // namespace BeastsOfChaos
