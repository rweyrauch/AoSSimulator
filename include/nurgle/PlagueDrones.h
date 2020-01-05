/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEDRONES_H
#define PLAGUEDRONES_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle
{

class PlagueDrones : public Unit
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 800;

    enum WeaponOption
    {
        PrehensileProboscis,
        FoulMouthparts
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    PlagueDrones();
    ~PlagueDrones() override = default;

    bool configure(int numModels, WeaponOption weapon, bool iconBearer, bool bellTollers);

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;

protected:

    bool m_iconBearer = false;
    bool m_bellTollers = false;
    WeaponOption m_weapon = PrehensileProboscis;

private:

    Weapon m_plaguesword,
        m_plagueswordPlaguebringer,
        m_deathsHead,
        m_proboscis,
        m_mouthparts,
        m_venemousSting;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Locus of Contagion               No
// Icon Bearer                      Yes
// Bell Toller                      No
//

} // Nurgle

#endif //PLAGUEBEARERS_H