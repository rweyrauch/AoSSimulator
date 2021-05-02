/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class ArchWarlock : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ArchWarlock();

        ~ArchWarlock() override = default;

    protected:

        void configure(Lore lore);

        void onRestore() override;

        void onEndCombat(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_halberd,
                m_claw;

        mutable bool m_moreMoreFailed = false;
        bool m_usedGauntlet = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// More-more Stormcage!             Yes
// Warpfire Gauntlet                Yes
// Warp Lightning Storm             Yes
//

} // namespace Skaven
