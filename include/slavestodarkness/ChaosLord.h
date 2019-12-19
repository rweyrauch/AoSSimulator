/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSLORD_H
#define CHAOSLORD_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosLord : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 110;

    enum WeaponOption
    {
        Reaperblade,
        DaemonboundSteel,
        DaemonboundWarFlail
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    ChaosLord();
    ~ChaosLord() override = default;

    bool configure(WeaponOption option);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_blade,
        m_steel,
        m_flail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Daemonbound                      No
// Spurred by the Gods              No
// Mark of Chaos                    No
//

} // SlavesToDarkness

#endif //CHAOSLORD_H