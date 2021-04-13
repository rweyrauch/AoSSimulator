/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class Prosecutors : public StormcastEternal {
    public:

        enum WeaponOption {
            Stormcall_Javelin_And_Shield,
            Paired_Celestial_Hammers,
            Celestial_Hammer_And_Shield
        };

        enum GrandWeaponOption {
            No_Grand_Weapon,
            Stormsurge_Trident,
            Grandaxe,
            Grandblade,
            Grandhammer,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Prosecutors(Stormhost stormhost, int numModels, WeaponOption weapons, GrandWeaponOption primeGrandWeapon,
                    int numTridents, int numGrandaxes, int numGrandblades, int numGrandhammers);

        ~Prosecutors() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int rollChargeDistance() override;

    private:

        WeaponOption m_weaponOption = Stormcall_Javelin_And_Shield;

        Weapon m_celestialHammersMissile,
                m_stormcallJavelinMissile,
                m_stormcallJavelinMissilePrime,
                m_stormsurgeTridentMissile,
                m_stormsurgeTridentMissilePrime,
                m_celestialHammers,
                m_celestialHammersPrime,
                m_grandaxe,
                m_grandblade,
                m_grandhammer,
                m_stormcallJavelin,
                m_stormsurgeTrident;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cleaving Blow                    Yes
// Heralds of Righteousness         Yes
// Paired Celestial Hammers         Yes
// Sigmarite Shields                Yes
// Stormcall Javelins               Yes
//

} // namespace StormcastEternals
