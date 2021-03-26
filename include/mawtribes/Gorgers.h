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

    class Gorgers : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Gorgers() = delete;

        ~Gorgers() override = default;

    protected:

        Gorgers(Mawtribe tribe, int numModels);

    protected:

        // Insatiable Hunger
        Rerolls chargeRerolls() const override { return Rerolls::Failed; }

    private:

        Weapon m_claws{Weapon::Type::Melee, "Long Claws", 1, 4, 3, 3, 0, 2},
                m_jaw{Weapon::Type::Melee, "Distensible Jaw", 1, 1, 3, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ambushing Hunters                TODO
// Insatiable Hunger                Yes

} // namespace OgorMawtribes

