/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class FomoroidCrusher : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit FomoroidCrusher(DamnedLegion legion);

        FomoroidCrusher() = delete;

        ~FomoroidCrusher() override = default;

    protected:

        void onCharged() override;

        int rollChargeDistance() override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_hurledTerrain{Weapon::Type::Missile, "Hurled Terrain", 12, 2, 3, 3, -1, 2},
                m_fists{Weapon::Type::Melee, "Crushing Fists", 1, 4, 3, 3, 0, 2};

        mutable int m_lastChargeDistance = 0;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rampage                          Yes
// Insurmountable Strength          TODO
//

} // SlavesToDarkness

