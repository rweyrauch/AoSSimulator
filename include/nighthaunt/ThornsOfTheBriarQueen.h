/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class ThornsOfTheBriarQueen : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ThornsOfTheBriarQueen();

        ~ThornsOfTheBriarQueen() override = default;

    protected:

        void configure();

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_malignantWeapon,
            m_malignantWeaponVarclav;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Grasping Chains                  TODO

} // namespace Nighthaunt

