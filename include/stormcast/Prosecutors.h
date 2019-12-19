/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PROSECUTORS_H
#define PROSECUTORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Prosecutors : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    enum WeaponOption
    {
        StormcallJavelinAndShield,
        PairedCelestialHammers,
        CelestialHammerAndShield
    };

    enum GrandWeaponOption
    {
        None,
        StormsurgeTrident,
        Grandaxe,
        Grandblade,
        Grandhammer,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    Prosecutors();
    ~Prosecutors() override = default;

    bool configure(int numModels, WeaponOption weapons, GrandWeaponOption primeGrandWeapon,
        int numTridents, int numGrandaxes, int numGrandblades, int numGrandhammers);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    WeaponOption m_weapon = StormcallJavelinAndShield;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cleaving Blow                    Yes
// Heralds of Righteousness         No
// Paired Celestial Hammers         Yes
// Sigmarite Shields                Yes
// Stormcall Javelins               Yes
//

} // namespace StormcastEternals

#endif //PROSECUTORS_H