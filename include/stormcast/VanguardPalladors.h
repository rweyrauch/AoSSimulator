/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANGUARDPALLADORS_H
#define VANGUARDPALLADORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VanguardPalladors : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    enum WeaponOption
    {
        ShockHandaxe,
        StarstrikeJavelin,
    };

    static Unit *Create(const ParameterList &parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    VanguardPalladors();
    ~VanguardPalladors() override = default;

    bool configure(int numModels, WeaponOption weapons);

protected:

    Wounds onEndCombat(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_boltstormPistol,
        m_starstrikeJavelinMissile,
        m_shockHandaxe,
        m_starstrikeJavelin,
        m_beakAndClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Aetherial Strike                 Yes
// Lunar Blade                      Yes
// Ride the Wind Aetheric           No
//

} // namespace StormcastEternals

#endif //VANGUARDPALLADORS_H