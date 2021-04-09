/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class ChaosGargant : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosGargant();

        ~ChaosGargant() override = default;

        explicit ChaosGargant(Greatfray fray);

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_eadbutt{Weapon::Type::Melee, "Vicious 'eadbutt", 1, 1, 4, 3, -3, 6},
                m_massiveClub{Weapon::Type::Melee, "Massive Club", 3, RAND_3D6, 3, 3, -1, 1},
                m_mightyKick{Weapon::Type::Melee, "Mighty Kick", 2, 1, 3, 3, -2, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Whipped into Frenzy              Yes
// Drunken Stagger                  TODO
// Stuff 'Em In Me Bag              Yes
// Timber!                          TODO
//

} // namespace GloomspiteGitz
