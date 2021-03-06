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

    class DankholdTroggoths : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DankholdTroggoths(Allegiance allegiance, int numModels, int points);

        ~DankholdTroggoths() override;

    protected:

        void onStartCombat(PlayerId player) override;

        void onStartHero(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int reassuringPresence(const Unit *unit);

    private:

        Weapon m_boulderClub;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crushing Grip                    Yes
// Magical Resistance               Yes
// Reassuring Presence              Yes
// Regeneration                     Yes
// Squiggly-beast Followers         Yes
//

} // namespace GloomspiteGitz
