/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PUTRIDBLIGHTKINGS_H
#define PUTRIDBLIGHTKINGS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class PutridBlightkings : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        PutridBlightkings();

        ~PutridBlightkings() override = default;

        bool configure(int numModels, bool iconBearer, bool sonorousTocsin);

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        void onStartHero(PlayerId player) override;

    protected:

        bool m_iconBearer = false;
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

#endif //PUTRIDBLIGHTKINGS_H