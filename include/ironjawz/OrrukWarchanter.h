/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "Ironjawz.h"
#include <Weapon.h>

namespace Ironjawz {

    class OrrukWarchanter : public Ironjawz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        OrrukWarchanter();

        ~OrrukWarchanter() override = default;

    protected:

        void configure(Warbeat warbeat);

        void onStartHero(PlayerId player) override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_stikks;
        Warbeat m_warbeat = Warbeat::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rhythm of Destruction            Yes
// Violent Fury                     Yes

} // namespace Ironjawz
