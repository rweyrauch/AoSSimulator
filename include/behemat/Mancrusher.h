/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class Mancrusher : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Mancrusher();

        ~Mancrusher() override = default;

    private:

        bool configure(int numModels);

        void onWounded() override;

        void onRestore() override;

        void onStartShooting(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        size_t getDamageTableIndex() const;

        Weapon m_eadbutt,
                m_club,
                m_kick,
                m_rocks;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Keep Up!                         TODO
// Stomping Charge                  TODO
// Stuff 'Em In Me Bag              TODO
// Timber!                          TODO
// Getting Stuck In                 Yes
// Breaking Down the Houses         TODO
//

} // namespace SonsOfBehemat
