/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ironjawz/Ironjawz.h>

namespace Ironjawz {

    class GordrakkTheFistOfGork : public Ironjawz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GordrakkTheFistOfGork();

        ~GordrakkTheFistOfGork() override = default;

    protected:

        void configure();

        void onWounded() override;

        void onCharged() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

    private:

        size_t getDamageTableIndex() const;

        int m_modelsSlainAtStartOfCombat = 0;

        Weapon m_bellow,
                m_smasha,
                m_kunnin,
                m_fistsAndTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Massively Destructive Bulk       Yes
// Kunnin                           Yes
// Smasha                           Yes
// Strength from Victory            Yes
// Voice of Gork                    Yes
//

} // namespace Ironjawz
