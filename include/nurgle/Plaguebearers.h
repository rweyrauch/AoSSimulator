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

    class Plaguebearers : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Plaguebearers(int points);

        ~Plaguebearers() override = default;

        bool configure(int numModels, bool iconBearer, bool pipers);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_plaguesword,
                m_plagueswordPlagueRidden;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Cloud of Flies                   Yes
// Locus of Fecundity               Yes
//

} // Nurgle
