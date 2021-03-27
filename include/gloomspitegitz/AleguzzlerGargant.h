/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class AleguzzlerGargant : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AleguzzlerGargant();

        ~AleguzzlerGargant() override = default;

    protected:

        void configure();

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_eadbutt,
                m_massiveClub,
                m_mightyKick;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drunken Stagger                  TODO
// Stuff 'Em In Me Bag              Yes
// Timber!                          TODO
//

} // namespace GloomspiteGitz
