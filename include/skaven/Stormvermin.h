/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Stormvermin : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Stormvermin(int numModels, bool clanshields, int standardBearers, int drummers, int points);

        ~Stormvermin() override = default;

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int runModifier() const override;

    private:

        bool m_clanshields = false;
        int m_numStandardBearers = 0;
        int m_numDrummers = 0;

        Weapon m_rustyHalberd,
                m_rustyHalberdLeader;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Drummer                          Yes
// Clanshields                      Yes

} // namespace Skaven
