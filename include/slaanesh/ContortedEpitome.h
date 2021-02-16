/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class TheContortedEpitome : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheContortedEpitome();

        ~TheContortedEpitome() override = default;

        bool configure(Lore lore);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        // Gift of Power
        Rerolls castingRerolls() const override { return Reroll_Failed; }
    private:

        Weapon m_ravagingClaws,
                m_coiledTentacles;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gift of Power                    Yes
// Swallow Energy                   Yes
// Horrible Fascination             TODO
// Overwhelming Acquiescence        Yes
//

} // Slannesh
