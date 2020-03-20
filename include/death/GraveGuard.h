/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRAVEGUARD_H
#define GRAVEGUARD_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class GraveGuard : public LegionOfNagashBase
{
public:

    enum WeaponOptions
    {
        WightBlade,
        GreatWightBlade,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    GraveGuard();
    ~GraveGuard() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    bool m_standardBearers = false;
    bool m_hornblowers = false;
    WeaponOptions m_weaponOption = WightBlade;

    Weapon m_wightBlade,
        m_wightBladeSeneschal,
        m_greatWightBlade,
        m_greatWightBladeSeneschal;

    static bool s_registered;

};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  TODO
// Hornblower                       TODO
// Cursed Weapons                   Yes
// Crypt Shields                    Yes
//

} //namespace Death

#endif //GRAVEGUARD_H
