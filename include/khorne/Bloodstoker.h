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

        static int ComputePoints(int numModels);

        static void Init();

        Bloodstoker();

        ~Bloodstoker() override = default;

    protected:

        void configure();

        void onStartMovement(PlayerId player) override;

    private:

        Weapon m_tortureBlade,
                m_bloodWhip;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Whipped to Fury                  Yes
//

} // namespace Khorne
