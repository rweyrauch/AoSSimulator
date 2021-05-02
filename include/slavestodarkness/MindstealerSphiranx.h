/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class MindstealerSphiranx : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit MindstealerSphiranx(DamnedLegion legion);

        MindstealerSphiranx() = delete;

        ~MindstealerSphiranx() override;

    protected:

        int telepathicDread(const Unit *unit);

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_claws{Weapon::Type::Melee, "Shredding Claws", 1, 3, 3, 3, -1, 1},
                m_tail{Weapon::Type::Melee, "Lashing Tail", 1, 2, 4, 3, 0, 1};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Telepathic Dread                 Yes
// Dominate Mind                    Yes
//

} // SlavesToDarkness

