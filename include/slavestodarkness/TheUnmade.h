/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class TheUnmade : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheUnmade(DamnedLegion legion, int numModels);

        TheUnmade() = delete;

        ~TheUnmade() override;

    protected:

        int frozenInFear(const Unit *unit);

    private:

        Weapon  m_maimingWeapons{Weapon::Type::Melee, "Maiming Weapons", 1, 1, 4, 4, 0, 1},
                m_maimingWeaponsLeader{Weapon::Type::Melee, "Maiming Weapons (Joyous One)", 1, 2, 4, 4, 0, 1},
                m_nigthmareSickles{Weapon::Type::Melee, "Nightmare Sickles", 1, 3, 4, 3, -1, 2};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frozen in Fear                   Partial/TODO
//

} // SlavesToDarkness
