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

    class GrimghastReapers : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GrimghastReapers();

        ~GrimghastReapers() override = default;

        bool configure(int numModels);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_slasherScythe,
                m_deathKnell;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Reaped Like Core                 Yes
// For Whom the Bell Tolls          TODO
//

} // namespace Nighthaunt

