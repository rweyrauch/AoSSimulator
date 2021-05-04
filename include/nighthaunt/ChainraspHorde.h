/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class ChainraspHorde : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChainraspHorde(Procession procession, int numModels, int points);

        ~ChainraspHorde() override = default;

    protected:

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_malignantWeapon,
                m_malignantWeaponWarden;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Chilling Horde                   Yes

} // namespace Nighthaunt

