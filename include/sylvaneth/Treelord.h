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

    class Treelord : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Treelord(Glade glade);

        Treelord() = delete;

        ~Treelord() override = default;

    protected:

        void onWounded() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        size_t getDamageTableIndex() const;

        void onStartCombat(PlayerId id) override;

    private:

        Weapon m_strangleroots,
                m_sweepingBlows,
                m_massiveImpalingTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              Yes
// Impale                           Yes
// Spirit Path                      TODO
//

} // namespace Sylvaneth
