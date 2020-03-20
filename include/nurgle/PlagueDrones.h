/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEDRONES_H
#define PLAGUEDRONES_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class PlagueDrones : public NurgleBase
{
public:

    enum WeaponOption
    {
        PrehensileProboscis,
        FoulMouthparts
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    PlagueDrones();
    ~PlagueDrones() override;

    bool configure(int numModels, WeaponOption weapon, bool iconBearer, bool bellTollers);

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

protected:

    bool m_iconBearer = false;
    bool m_bellTollers = false;
    WeaponOption m_weapon = PrehensileProboscis;

    Rerolls bellTollersBattleshockReroll(const Unit* unit);

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

#endif //PLAGUEBEARERS_H