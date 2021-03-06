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
#include <gloomspitegitz/GloomspiteSpells.h>

namespace GloomspiteGitz {

    class Mollog : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Mollog(Allegiance allegiance, bool isGeneral);

        ~Mollog() override;

    protected:

        void onStartHero(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onRestore() override;

        int reassuringPresence(const Unit *unit);

    private:

        Weapon m_jabbertoad,
                m_club;

        lsignal::slot m_connection;

        bool m_batSquig = true;
        bool m_spiteshroom = true;
        bool m_stalagsquig = true;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Jabbertoad                       TODO
// Puff-fungus Club                 TODO
// Magical Resistance               Yes
// Reassuring Presence              Yes
// Regeneration                     Yes
// Bat Squig                        Yes
// Spiteshroom                      Yes
// Stalagsquig                      Yes
// Loyal to the End                 Yes
//

} // namespace GloomspiteGitz
