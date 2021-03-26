/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Firebelly : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Firebelly() = delete;

        ~Firebelly() override = default;

    protected:

        Firebelly(Mawtribe tribe, Lore lore, bool isGeneral);

    protected:

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_fireBreath{Weapon::Type::Missile, "Fire Breath", 6, 1, 0, 0, 0, 0},
                m_hammer{Weapon::Type::Melee, "Basalt Hammer", 2, 2, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fire Breath                      Yes
// Cascading Fire-cloak             TODO
//

} // namespace OgorMawtribes
