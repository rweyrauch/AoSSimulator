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

    class VerminlordWarpseer : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VerminlordWarpseer();

        ~VerminlordWarpseer() override;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartHero(PlayerId player) override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        void onStartShooting(PlayerId player) override;

        void onWounded() override;

        void onRestore() override;

        int terrifying(const Unit *target);

        int getDamageTableIndex() const;

    private:

        Weapon m_tails,
                m_glaive;

        bool m_usedOrb = false;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Great Manipulators           Yes
// Protection of the Horned Rat     Yes
// Scry-orb                         TODO
// Terrifying                       Yes
// Dreaded Warpgale                 Yes
// Forth-forth, Children of the Horned Rat! TODO
//

} // namespace Skaven
