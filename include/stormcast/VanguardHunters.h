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

namespace StormcastEternals {

    class VanguardHunters : public StormcastEternal {
    public:

        enum WeaponOption {
            ShockHandaxe,
            StormSabre,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        VanguardHunters();

        ~VanguardHunters() override = default;

        bool configure(int numModels, WeaponOption weapons, bool astralCompass);

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
// Abilities                    Implemented
// -------------------------------------------
// Astral Compass                   TODO
// Tireless Hunters                 Yes
//

} // namespace StormcastEternals

#endif //VANGUARDHUNTERS_H