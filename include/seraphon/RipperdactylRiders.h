/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class RipperdactylRiders : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RipperdactylRiders(WayOfTheSeraphon way, Constellation constellation, int numModels, int points);

        RipperdactylRiders() = delete;

        ~RipperdactylRiders() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRestore() override { m_usedToad = false; }

    private:

        mutable bool m_usedToad = false;

        Weapon  m_spear{Weapon::Type::Melee, "Moonstone Warspear", 1, 1, 4, 4, 0, 1},
                m_spearAlpha{Weapon::Type::Melee, "Moonstone Warspear", 1, 2, 4, 4, 0, 1},
                m_jaws{Weapon::Type::Melee, "Tearing Jaws", 1, 3, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Voracious Appetite               Yes
// Toad Rage                        Yes
//

} // namespace Seraphon
