/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class WightKingWithBalefulTombBlade : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        WightKingWithBalefulTombBlade();

        ~WightKingWithBalefulTombBlade() override = default;

    protected:

        void configure(bool hasSteed);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

    private:

        bool m_hasSteed = false;

        Weapon m_balefulTombBlade,
                m_steedsHoovesAndTeeth;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Beheading Strike                 Yes
// Deathly Invocation               Yes
// Lord of Bones                    TODO
//

} // namespace Death
