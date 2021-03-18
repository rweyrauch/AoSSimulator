/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class OrghottsDaemonspew : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OrghottsDaemonspew();

        ~OrghottsDaemonspew() override = default;

        bool configure();

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_tongue,
                m_rotaxes,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Acid Ichor                       TODO
// Fury of the Halfblood            Yes
// The Rotaxes                      TODO
// Fester and Rot                   TODO
//

} // Nurgle
