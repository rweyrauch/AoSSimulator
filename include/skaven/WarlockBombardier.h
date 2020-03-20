/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARLOCKBOMBARDIER_H
#define WARLOCKBOMBARDIER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class WarlockBombardier : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        WarlockBombardier();

        ~WarlockBombardier() override = default;

        bool configure();

    protected:

        void onRestore() override;

        Wounds onEndShooting(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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

#endif //WARLOCKBOMBARDIER_H