/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class ScreamersOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ScreamersOfTzeentch();

        ~ScreamersOfTzeentch() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_bite;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sky-sharks                       Yes
// Slashing Fins                    TODO
//

} // Tzeentch
