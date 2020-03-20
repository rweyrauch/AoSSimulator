/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PUSGOYLEBLIGHTLORDS_H
#define PUSGOYLEBLIGHTLORDS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class PusgoyleBlightlords : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        PusgoyleBlightlords();

        ~PusgoyleBlightlords() override = default;

        bool configure(int numModels, int numTocsins);

    protected:

        Wounds applyWoundSave(const Wounds &wounds) override;

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

#endif //PUSGOYLEBLIGHTLORDS_H