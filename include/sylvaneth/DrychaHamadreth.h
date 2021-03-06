/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class DrychaHamadreth : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DrychaHamadreth(Glade glade, Lore lore, bool isGeneral);

        DrychaHamadreth() = delete;

        ~DrychaHamadreth() override;

    protected:

        void onWounded() override;

        size_t getDamageTableIndex() const;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onBeginRound(int battleRound) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls songOfSpiteToWoundRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        bool m_enraged = false;

        Weapon m_colonyOfFlitterfuries,
                m_swarmOfSquirmlings,
                m_slashingTalons,
                m_thornedSlendervines;

        lsignal::slot m_songSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Infestation               Yes
// Mercurial Aspect                 Yes
// Song of Spite                    Yes
// Primal Terror                    Yes
//

} // namespace Sylvaneth
