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

    class NightRunners : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        NightRunners(int numModels, int points);

        ~NightRunners() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_throwingWeapons,
                m_stabbingBlade,
                m_stabbingBladeLeader;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Running Death                    Yes
// Slinking Advance                 TODO
// Throwing Weapons                 Yes
//

} // namespace Skaven
