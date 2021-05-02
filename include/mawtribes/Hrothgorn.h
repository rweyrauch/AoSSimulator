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

    class Hrothgorn : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Hrothgorn() = delete;

        ~Hrothgorn() override = default;

    protected:

        Hrothgorn(bool isGeneral);

    private:

        Weapon m_trapLauncher{Weapon::Type::Missile, "Trap Launcher", 12, 1, 4, 3, 0, RAND_D3},
                m_knife{Weapon::Type::Melee, "Hunting Knife", 1, 4, 3, 3, 0, 2},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Masters of Ambush                TODO
// Thrafnir                         TODO
//

} // namespace OgorMawtribes
