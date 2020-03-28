/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MOLLOG_H
#define MOLLOG_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz {

    class Mollog : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Mollog();

        ~Mollog() override;

        bool configure();

    protected:

        int reassuringPresence(const Unit *unit);

    private:

        Weapon m_jabbertoad,
            m_club;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Jabbertoad                       TODO
// Puff-fungus Club                 TODO
// Magical Resistance               TODO
// Reassuring Presence              Yes
// Regeneration                     TODO
// Bat Squig                        TODO
// Spiteshroom                      TODO
// Stalagsquig                      TODO
// Loyal to the End                 TODO
//

} // namespace GloomspiteGitz

#endif //MOLLOG_H