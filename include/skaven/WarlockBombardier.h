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

    class WarlockBombardier : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WarlockBombardier();

        ~WarlockBombardier() override = default;

    protected:

        void configure(Lore lore);

        void onRestore() override;

        void onEndShooting(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_doomrocket,
                m_pole;

        mutable bool m_moreMoreFailed = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// More-more Doomrocket!            Yes
// Warp Lightning                   Yes
//

} // namespace Skaven
