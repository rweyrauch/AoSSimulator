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

        static int ComputePoints(int numModels);

        static void Init();

        Skarbrand();

        ~Skarbrand() override = default;

    protected:

        void configure();

        size_t getDamageTableIndex() const;

        void onWounded() override;

        Rerolls chargeRerolls() const override;

        int generateMortalWounds(const Unit *unit) override;

        void onStartShooting(PlayerId player) override;

    private:

        bool m_attackedInPreviousRound = false;

        Weapon m_slaughter,
                m_carnage;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Skarbrand's Rage                 TODO
// Roar of Total Rage               Yes
// Total Carnage                    Yes
// Inescapable Wrath                Yes
//

} // namespace Khorne
