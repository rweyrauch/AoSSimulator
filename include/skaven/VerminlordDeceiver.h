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

    class VerminlordDeceiver : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VerminlordDeceiver();

        ~VerminlordDeceiver() override;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onWounded() override;

        void onRestore() override;

        int terrifying(const Unit *target);

        size_t getDamageTableIndex() const;

    private:

        Weapon m_doomstar,
                m_tails,
                m_warpstiletto;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Doomstar                         Yes
// Protection of the Horned Rat     Yes
// Shrouded In Darkness             Yes
// Terrifying                       Yes
// Dreaded Skitterleap              TODO
// Lord of Assassins                TODO
//

} // namespace Skaven
