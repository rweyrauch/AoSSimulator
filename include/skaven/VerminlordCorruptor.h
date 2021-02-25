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

    class VerminlordCorruptor : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VerminlordCorruptor();

        ~VerminlordCorruptor() override;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        void onWounded() override;

        void onRestore() override;

        int terrifying(const Unit *target);

        int getDamageTableIndex() const;

    private:

        Weapon m_tails,
                m_plaguereapers;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Plaguereapers                    Yes
// Plaguemaster                     Yes
// Protection of the Horned Rat     Yes
// Terrifying                       Yes
// Dreaded Plague                   Yes
// Lord of Pestilence               TODO
//

} // namespace Skaven
