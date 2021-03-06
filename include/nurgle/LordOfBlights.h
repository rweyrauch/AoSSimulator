/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class LordOfBlights : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordOfBlights(PlagueLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordOfBlights() override = default;

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_ripenedDeathsHead,
                m_bountyDeathsHead,
                m_hammer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Munificent Bounty                TODO
// Vermid Shield                    Yes
// Plague of Flies                  TODO
//

} // Nurgle
