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

    class RockgutTroggoths : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RockgutTroggoths(Allegiance allegiance, int numModels, int points);

        ~RockgutTroggoths() override = default;

    protected:

        void onStartHero(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_massiveStoneMaul;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Regeneration                     Yes
// Stony Skin                       Yes
// Throwin' Boulders                Yes
//

} // namespace GloomspiteGitz
