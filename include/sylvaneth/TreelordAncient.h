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

    class TreelordAncient : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TreelordAncient();

        ~TreelordAncient() override = default;

        bool configure(Lore lore);

    protected:

        void onWounded() override;

        int getDamageTableIndex() const;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_doomTendrilStaff,
                m_sweepingBlows,
                m_massiveImpalingTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              TODO
// Spirit Paths                     TODO
// Impale                           Yes
// Silent Communication             TODO
// Awakening the Wood               TODO
// Heed the Spirit-song             Yes
//

} // namespace Sylvaneth
