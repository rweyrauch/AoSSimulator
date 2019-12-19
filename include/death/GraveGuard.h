/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRAVEGUARD_H
#define GRAVEGUARD_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class GraveGuard : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum WeaponOptions
    {
        WightBlade,
        GreatWightBlade,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    GraveGuard();
    ~GraveGuard() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    bool m_standardBearers = false;
    bool m_hornblowers = false;
    WeaponOptions m_weapons = WightBlade;

    Weapon m_wightBlade,
        m_wightBladeSeneschal,
        m_greatWightBlade,
        m_greatWightBladeSeneschal;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  No
// Hornblower                       No
// Cursed Weapons                   Yes
// Crypt Shields                    Yes
//

} //namespace Death

#endif //GRAVEGUARD_H
