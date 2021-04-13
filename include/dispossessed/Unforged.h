/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed {

    class Unforged : public Dispossessed {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Unforged();

        ~Unforged() override = default;

    protected:

        void configure();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_runicAxes;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Runic Axes                       Yes
// Epic Deathblow                   TODO
// Nemesis                          Yes
// The Bigger They Are...           Yes
//

} // namespace Dispossessed
