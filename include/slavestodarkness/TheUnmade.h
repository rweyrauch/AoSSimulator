/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEUNMADE_H
#define THEUNMADE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class TheUnmade : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheUnmade();

        ~TheUnmade() override;

        bool configure(int numModels);

    protected:

        int frozenInFear(const Unit *unit);

    private:

        Weapon m_maimingWeapons,
                m_maimingWeaponsLeader,
                m_nigthmareSickles;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frozen in Fear                   Partial/TODO
//

} // SlavesToDarkness

#endif //THEUNMADE_H