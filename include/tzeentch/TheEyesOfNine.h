/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class TheEyesOfTheNine : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheEyesOfTheNine();

        ~TheEyesOfTheNine() override = default;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_flames,
                m_bolt,
                m_greatblade,
                m_blade,
                m_beak,
                m_hands;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcanite Shield                  Yes
// Split                            TODO
//

} // namespace Tzeentch

