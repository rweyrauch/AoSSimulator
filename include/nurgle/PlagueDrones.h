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

    class PlagueDrones : public NurgleBase {
    public:

        enum WeaponOption {
            Prehensile_Proboscis,
            Foul_Mouthparts
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PlagueDrones(int points);

        ~PlagueDrones() override;

        bool configure(int numModels, WeaponOption weapon, bool iconBearer, bool bellTollers);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    protected:

        WeaponOption m_weapon = Prehensile_Proboscis;

        Rerolls bellTollersBattleshockReroll(const Unit *unit);

    private:

        Weapon m_plaguesword,
                m_plagueswordPlaguebringer,
                m_deathsHead,
                m_proboscis,
                m_mouthparts,
                m_venemousSting;

        lsignal::slot m_bellTollerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Locus of Contagion               Yes
// Icon Bearer                      Yes
// Bell Toller                      Yes
//

} // Nurgle
