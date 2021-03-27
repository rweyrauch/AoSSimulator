/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class SymbareshTwinsouls : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SymbareshTwinsouls();

        ~SymbareshTwinsouls() override = default;

        bool configure(int numModels);

    protected:

        void onStartHero(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_blades,
                m_bladesEgopomp;

        bool m_fiendishReflexesActive = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fractured Souls
//   Ego-driven Excess              Yes
//   Fiendish Reflexes              Yes
//

} // Slannesh
