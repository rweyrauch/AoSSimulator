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

    class CypherLords : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CypherLords(DamnedLegion legion, int numModels, int points);

        CypherLords() = delete;

        ~CypherLords() override = default;

    protected:

        int chargeModifier() const override;

        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_throwingStars,
                m_exoticBlades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Luminate                         Yes
// Shattered Gloom Globe            Yes
//

} // SlavesToDarkness
