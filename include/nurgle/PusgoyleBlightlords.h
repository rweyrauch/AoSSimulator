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

    class PusgoyleBlightlords : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PusgoyleBlightlords(PlagueLegion legion, int numModels, int numTocsins, int points);

        ~PusgoyleBlightlords() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        void onStartHero(PlayerId player) override;

    protected:

    private:

        Weapon m_blightedWeapon,
                m_dolorousTocsin,
                m_mouthparts,
                m_venemousSting;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Virulent Discharge               Yes
// Blighted Weapons                 Yes
//

} // Nurgle
