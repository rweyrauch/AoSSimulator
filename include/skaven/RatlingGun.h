/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RATLINGGUN_H
#define RATLINGGUN_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class RatlingGun : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        RatlingGun();

        ~RatlingGun() override = default;

        bool configure();

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onRestore() override;

        void onEndShooting(PlayerId player) override;

    private:

        Weapon m_ratlingGun,
                m_rustyKnives;

        mutable bool m_moreMoreFailed = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// More-more Warplead!              Yes
//

} // namespace Skaven

#endif //RATLINGGUN_H