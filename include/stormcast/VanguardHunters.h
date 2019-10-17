/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANGUARDHUNTERS_H
#define VANGUARDHUNTERS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VanguardHunters : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum WeaponOption
    {
        ShockHandaxe,
        StormSabre,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    VanguardHunters();
    ~VanguardHunters() override = default;

    bool configure(int numModels, WeaponOption weapons, bool astralCompass);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    WeaponOption m_weaponOption = StormSabre;
    bool m_astralCompass = false;

    Weapon m_boltstormPistol,
                  m_boltstormPistolPrime,
                  m_shockHandaxe,
                  m_shockHandaxePrime,
                  m_stormSabre,
                  m_stormSabrePrime;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Astral Compass                   No
// Tireless Hunters                 Yes
//

} // namespace StormcastEternals

#endif //VANGUARDHUNTERS_H