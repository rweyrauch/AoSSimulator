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

    class CovenThrone : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CovenThrone();

        ~CovenThrone() override = default;

    protected:

        void configure(Lore lore);

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        void onStartHero(PlayerId player) override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_bite,
                m_stiletto,
                m_poniards,
                m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  Yes
// Scrying Pool                     TODO
// The Hunger                       Yes
// Deathly Invocation               Yes
// Beguile                          TODO
// Tactful Insight                  TODO
//

} // namespace Death
