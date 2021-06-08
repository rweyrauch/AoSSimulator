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

    class LordImperatant : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordImperatant(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordImperatant() override = default;

    protected:

    private:

        Weapon  m_baton{Weapon::Type::Missile, "Stormcaller Baton", 18, RAND_D6, 3, 3, -1, 1},
                m_warhammer{Weapon::Type::Melee, "Blessed Warhammer", 1, 4, 3, 3, -1, 2},
                m_beakAndClaws{Weapon::Type::Melee, "Vicious Beak and Claws", 1, 4, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Distinguished Leader             TODO
// Guided by Lightning              TODO
//

} // namespace StormcastEternals
