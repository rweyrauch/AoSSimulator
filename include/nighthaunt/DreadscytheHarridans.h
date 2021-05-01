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

    class DreadscytheHarridans : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit DreadscytheHarridans(int numModels);

        DreadscytheHarridans() = delete;

        ~DreadscytheHarridans() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon  m_scythedLimbs{Weapon::Type::Melee, "Scythed Limbs", 1, 3, 4, 3, -1, 1},
                m_scythedLimbsCrone{Weapon::Type::Melee, "Scythed Limbs", 1, 4, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Harrowing Shriek                 Yes
// Murderous Bloodlust              Yes
//

} // namespace Nighthaunt

