/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ExaltedHeroOfChaos : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ExaltedHeroOfChaos();

        ~ExaltedHeroOfChaos() override = default;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_blades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Dark Blessings                   Yes
// Glory-hungry Bladesman           Yes
// Thrice-damned Dagger             TODO
// Trail of Red Ruin                TODO
//

} // SlavesToDarkness
