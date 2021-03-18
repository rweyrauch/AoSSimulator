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

    class PrinceVhordrai : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        PrinceVhordrai();

        ~PrinceVhordrai() override = default;

        bool configure(Lore lore);

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        int getDamageTableIndex() const;

    private:

        Weapon m_bloodlance,
                m_maw,
                m_claws;

        bool m_usedChaliceOfBlood = false;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       Yes
// Chalice of Blood                 Yes
// Bloodlance Charge                Yes
// Breath of Shyish                 Yes
// Deathly Invocation               Yes
// Quickblood                       TODO
// Fist of Nagash                   TODO
//

} // namespace Death
