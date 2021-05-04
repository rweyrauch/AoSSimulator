/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Packmasters : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Packmasters(int numModels, int numCatchers, int points);

        ~Packmasters() override;

    protected:

        int crackTheWhip(const Unit *attacker, const Weapon *weapon, const Unit *target);

        int crackTheWhipBravery(const Unit *unit);

    private:

        Weapon m_whip,
                m_blade,
                m_catcher;

        lsignal::slot m_whipSlot, m_whipBraverySlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crack the Whip                   Yes
//

} // namespace Skaven
