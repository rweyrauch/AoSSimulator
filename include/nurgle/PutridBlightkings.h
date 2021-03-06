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

    class PutridBlightkings : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PutridBlightkings(PlagueLegion legion, int numModels, bool iconBearer, bool sonorousTocsin, int points);

        ~PutridBlightkings() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        void onStartHero(PlayerId player) override;

    protected:

        bool m_sonorousTocsin = false;

    private:

        Weapon m_blightedWeapon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     Yes
// Sonorus Tocsin                   Yes
// Virulent Discharge               Yes
// Blighted Weapons                 Yes
//

} // Nurgle
