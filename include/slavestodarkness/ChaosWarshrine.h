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

    class ChaosWarshrine : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosWarshrine();

        ~ChaosWarshrine() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        void onStartHero(PlayerId player) override;

        int getDamageTableIndex() const;

    private:

        Weapon m_blade,
                m_fists;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Protection of the Dark Gods      TODO
// Favour of the Ruinous Powers
//   Favour of Khorne               Yes
//   Favour of Tzeentch             TODO
//   Favour of Nurgle               Yes
//   Favour of Slaanesh             Yes
//   Favour of Chaos                Yes
//

} // SlavesToDarkness
