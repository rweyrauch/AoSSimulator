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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChaosWarshrine(DamnedLegion legion, MarkOfChaos mark);

        ChaosWarshrine() = delete;

        ~ChaosWarshrine() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        void onStartHero(PlayerId player) override;

        size_t getDamageTableIndex() const;

        Wounds protectionOfTheDarkGods(const Wounds &wounds, const Unit *target, const Unit *attacker);

    private:

        lsignal::slot m_protectionSlot;

        Weapon  m_blade{Weapon::Type::Melee, "Sacrificial Blade", 1, 4, 3, 3, -1, 2},
                m_fists{Weapon::Type::Melee, "Flailing Fists", 1, 6, 4, 3, 0, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Protection of the Dark Gods      Yes
// Favour of the Ruinous Powers
//   Favour of Khorne               Yes
//   Favour of Tzeentch             TODO
//   Favour of Nurgle               Yes
//   Favour of Slaanesh             Yes
//   Favour of Chaos                Yes
//

} // SlavesToDarkness
