/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Skarbrand : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Skarbrand(SlaughterHost host, bool isGeneral);

        Skarbrand() = delete;

        ~Skarbrand() override = default;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        Rerolls chargeRerolls() const override;

        int generateMortalWounds(const Unit *unit) override;

        void onStartShooting(PlayerId player) override;

        void onEndRound(int battleRound) override;

    private:

        bool m_attackedInPreviousRound = false;

        Weapon  m_slaughter{Weapon::Type::Melee, "Slaughter", 2, 5, 4, 3, -2, 3},
                m_carnage{Weapon::Type::Melee, "Carnage", 2, 1, 4, 0, 0, 0};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Skarbrand's Rage                 Yes
// Roar of Total Rage               Yes
// Total Carnage                    Yes
// Inescapable Wrath                Yes
//

} // namespace Khorne
