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

    class Leadbelchers : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Leadbelchers() = delete;

        ~Leadbelchers() override = default;

    protected:

        Leadbelchers(Mawtribe tribe, int numModels);

    protected:

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_gun{Weapon::Type::Missile, "Leadbelcher Gun", 12, RAND_D3, 4, 3, -1, 1},
                m_blow{Weapon::Type::Melee, "Bludgeoning Blow", 1, 2, 3, 3, -1, 2},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1},
                m_blowThunderfist{Weapon::Type::Melee, "Bludgeoning Blow", 1, 3, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thunderous Blasts of Hot Metal   Yes
//

} // namespace OgorMawtribes

