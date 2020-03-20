/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSLORD_H
#define CHAOSLORD_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosLord : public SlavesToDarknessBase
{
public:

    enum WeaponOption
    {
        Reaperblade,
        DaemonboundSteel,
        DaemonboundWarFlail
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    ChaosLord();
    ~ChaosLord() override = default;

    bool configure(WeaponOption option);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_blade,
        m_steel,
        m_flail;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Daemonbound                      Yes
// Spurred by the Gods              TODO
// Mark of Chaos                    Yes
//

} // SlavesToDarkness

#endif //CHAOSLORD_H