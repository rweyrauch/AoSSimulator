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

        Mancrusher() = delete;

        Mancrusher(Tribe tribe, int numModels, FierceLoathing loathing);

        ~Mancrusher() override = default;

    private:

        void onWounded() override;

        void onRestore() override;

        void onStartShooting(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        size_t getDamageTableIndex() const;

        Weapon m_eadbutt{Weapon::Type::Melee, "'Eadbutt", 1, 1, 4, 3, -3, 4},
               m_club{Weapon::Type::Melee, "Massive Club", 3, 10, 3, 3, -1, 1},
               m_kick{Weapon::Type::Melee, "Mighty Kick", 2, 1, 3, 3, -2, RAND_D3},
               m_rocks{Weapon::Type::Missile, "Chuck Rocks", 18, RAND_D3, 4, 3, -1, RAND_D3};

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
