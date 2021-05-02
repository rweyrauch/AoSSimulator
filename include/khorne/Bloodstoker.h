/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Bloodstoker : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Bloodstoker() = delete;

        ~Bloodstoker() override = default;

    protected:

        Bloodstoker(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        void onStartMovement(PlayerId player) override;

    private:

        Weapon  m_tortureBlade{Weapon::Type::Melee, "Torture Blade", 1, 3, 3, 3, 0, 1},
                m_bloodWhip{Weapon::Type::Melee, "Blood Whip", 3, 3, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Whipped to Fury                  Yes
//

} // namespace Khorne
