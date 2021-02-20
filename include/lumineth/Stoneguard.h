/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class AlarithStoneguard : public LuminethBase {
    public:

        enum class WeaponOption : int {
            Stone_Mallet,
            Diamondpick_Hammer
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        AlarithStoneguard();

        ~AlarithStoneguard() override = default;

        bool configure(int numModels, WeaponOption weaponOption, bool standardBearer);

    protected:

        Rerolls battleshockRerolls() const override;
        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_malletOrHammer,
                m_stratumHammer,
                m_pairedStratumHammers;

        WeaponOption m_weaponOption = WeaponOption::Stone_Mallet;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Crushing Blow                    Yes
// Diamondpick Hammer               Yes
// Pair of Stratum Hammers          Yes

} // namespace LuminethRealmLords

