/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef ARCHWARLOCK_H
#define ARCHWARLOCK_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class ArchWarlock : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ArchWarlock();

        ~ArchWarlock() override = default;

        bool configure(Lore lore);

    protected:

        void onRestore() override;

        Wounds onEndCombat(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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

#endif //ARCHWARLOCK_H