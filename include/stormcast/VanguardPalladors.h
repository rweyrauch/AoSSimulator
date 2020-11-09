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

namespace StormcastEternals {

    class VanguardPalladors : public StormcastEternal {
    public:

        enum WeaponOption {
            Shock_Handaxe,
            Starstrike_Javelin,
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

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
// Abilities                    Implemented
// -------------------------------------------
// Aetherial Strike                 Yes
// Lunar Blade                      Yes
// Ride the Wind Aetheric           TODO
//

} // namespace StormcastEternals

#endif //VANGUARDPALLADORS_H