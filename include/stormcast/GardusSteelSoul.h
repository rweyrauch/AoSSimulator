/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class GardusSteelSoul : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit GardusSteelSoul(bool isGeneral);

        ~GardusSteelSoul() override = default;

    protected:

        void onStartShooting(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon  m_runeblade{Weapon::Type::Melee, "Sigmarite Runeblade", 1, 4, 3, 3, -1, 1},
                m_warhammer{Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Purity                   Yes
// Martyr's Strength                TODO
// Saintly Assault                  TODO
// Sigmarite Warcloak               Yes
// Furious Retribution              Yes
//

} // namespace StormcastEternals
