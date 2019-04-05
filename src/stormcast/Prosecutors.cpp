/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Prosecutors.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Prosecutors::Create,
    Prosecutors::ValueToString,
    Prosecutors::EnumStringToInt,
    {
        {ParamType::Integer, "Models", Prosecutors::MIN_UNIT_SIZE, Prosecutors::MIN_UNIT_SIZE, Prosecutors::MAX_UNIT_SIZE, Prosecutors::MIN_UNIT_SIZE},
        {ParamType::Enum, "Weapons", Prosecutors::StormcallJavelinAndShield, Prosecutors::StormcallJavelinAndShield, Prosecutors::CelestialHammerAndShield, 1},
        {ParamType::Enum, "Prime Grand Weapon", Prosecutors::None, Prosecutors::None, Prosecutors::Grandhammer, 1},
        {ParamType::Integer, "Stormsurge Tridents", 0, 0, Prosecutors::MAX_UNIT_SIZE/3, 1 },
        {ParamType::Integer, "Grandaxes", 0, 0, Prosecutors::MAX_UNIT_SIZE/3, 1 },
        {ParamType::Integer, "Grandblades", 0, 0, Prosecutors::MAX_UNIT_SIZE/3, 1 },
        {ParamType::Integer, "Grandhammers", 0, 0, Prosecutors::MAX_UNIT_SIZE/3, 1 }
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Prosecutors::s_registered = false;

Prosecutors::Prosecutors() :
    StormcastEternal("Prosecutors", 12, WOUNDS, 7, 4, true),
    m_celestialHammersMissile(Weapon::Type::Missile, "Celestial Hammer(s)", 18, 2, 4, 4, 0, 1),
    m_stormcallJavelinMissile(Weapon::Type::Missile, "Stormcall Javelin", 18, 1, 3, 3, 0, 1),
    m_stormcallJavelinMissilePrime(Weapon::Type::Missile, "Stormcall Javelin (Prime)", 18, 2, 3, 3, 0, 1),
    m_stormsurgeTridentMissile(Weapon::Type::Missile, "Stormsurge Trident", 18, 1, 3, 3, -1, 2),
    m_stormsurgeTridentMissilePrime(Weapon::Type::Missile, "Stormsurge Trident (Prime)", 18, 2, 3, 3, -1, 2),
    m_celestialHammers(Weapon::Type::Melee, "Celestial Hammers(s)", 1, 2, 3, 3, 0, 1),
    m_celestialHammersPrime(Weapon::Type::Melee, "Celestial Hammers(s) (Prime)", 1, 3, 3, 3, 0, 1),
    m_grandaxe(Weapon::Type::Melee, "Grandaxe", 1, 0, 3, 3, -1, 1),
    m_grandblade(Weapon::Type::Melee, "Grandblade", 1, 2, 3, 4, -1, 2),
    m_grandhammer(Weapon::Type::Melee, "Grandhammer", 1, 2, 4, 3, -1, 2),
    m_stormcallJavelin(Weapon::Type::Melee, "Stormcall Javelin", 2, 1, 4, 4, 0, 1),
    m_stormsurgeTrident(Weapon::Type::Melee, "Stormsurge Trident", 2, 1, 4, 4, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, ANGELOS, PROSECUTORS};
}

bool Prosecutors::configure(int numModels, Prosecutors::WeaponOption weapons, Prosecutors::GrandWeaponOption primeGrandWeapon, int numTridents, int numGrandaxes,
                            int numGrandblades, int numGrandhammers)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    int maxGrandWeapons = numModels / 3;
    int totalGrandWeapons = numGrandaxes + numGrandblades + numGrandhammers;

    int maxTridents = 0;
    int totalTridents = numTridents;

    if (primeGrandWeapon == StormsurgeTrident)
    {
        totalTridents++;
    }
    if (primeGrandWeapon == Grandaxe || primeGrandWeapon == Grandblade || primeGrandWeapon == Grandhammer)
    {
        totalGrandWeapons++;
    }

    // 1 in 3 can have a trident when using javelins
    if (weapons == StormcallJavelinAndShield)
    {
        maxTridents = numModels / 3;
        maxGrandWeapons = 0;
    }

    if (totalTridents > maxTridents)
    {
        return false;
    }
    if (totalGrandWeapons > maxGrandWeapons)
    {
        return false;
    }

    m_weapon = weapons;

    Model prime(BASESIZE, WOUNDS);
    switch (primeGrandWeapon)
    {
        case None:
            if (weapons == CelestialHammerAndShield || weapons == PairedCelestialHammers)
            {
                prime.addMissileWeapon(&m_celestialHammersMissile);
                prime.addMeleeWeapon(&m_celestialHammersPrime);
            }
            else if (weapons == StormcallJavelinAndShield)
            {
                prime.addMissileWeapon(&m_stormcallJavelinMissilePrime);
                prime.addMeleeWeapon(&m_stormcallJavelin);
            }
            break;
        case StormsurgeTrident:
            prime.addMissileWeapon(&m_stormsurgeTridentMissilePrime);
            prime.addMeleeWeapon(&m_stormsurgeTrident);
            totalTridents--;
            break;
        case Grandaxe:
            prime.addMeleeWeapon(&m_grandaxe);
            numGrandaxes--;
            break;
        case Grandblade:
            prime.addMeleeWeapon(&m_grandblade);
            numGrandblades--;
            break;
        case Grandhammer:
            prime.addMeleeWeapon(&m_grandhammer);
            numGrandhammers--;
            break;
    }
    addModel(prime);

    for (auto i = 0; i < totalTridents; i++)
    {
        Model tridentModel(BASESIZE, WOUNDS);
        tridentModel.addMissileWeapon(&m_stormsurgeTridentMissile);
        tridentModel.addMeleeWeapon(&m_stormsurgeTrident);
        addModel(tridentModel);
    }
    for (auto i = 0; i < numGrandaxes; i++)
    {
        Model grandaxeModel(BASESIZE, WOUNDS);
        grandaxeModel.addMeleeWeapon(&m_grandaxe);
        addModel(grandaxeModel);
    }
    for (auto i = 0; i < numGrandblades; i++)
    {
        Model grandbladeModel(BASESIZE, WOUNDS);
        grandbladeModel.addMeleeWeapon(&m_grandblade);
        addModel(grandbladeModel);
    }
    for (auto i = 0; i < numGrandhammers; i++)
    {
        Model grandhammerModel(BASESIZE, WOUNDS);
        grandhammerModel.addMeleeWeapon(&m_grandhammer);
        addModel(grandhammerModel);
    }
    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weapon == CelestialHammerAndShield || m_weapon == PairedCelestialHammers)
        {
            model.addMissileWeapon(&m_celestialHammersMissile);
            model.addMeleeWeapon(&m_celestialHammers);
        }
        else if (m_weapon == StormcallJavelinAndShield)
        {
            model.addMissileWeapon(&m_stormcallJavelinMissile);
            model.addMeleeWeapon(&m_stormcallJavelin);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Wounds Prosecutors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Stormcall Javelin
    if (weapon->name() == m_stormcallJavelin.name())
    {
        if (distanceTo(target) >= 9.0f)
        {
            return {weapon->damage()+1, 0};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void Prosecutors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_celestialHammersMissile);
    visitor(&m_stormcallJavelinMissile);
    visitor(&m_stormcallJavelinMissilePrime);
    visitor(&m_stormsurgeTridentMissile);
    visitor(&m_stormsurgeTridentMissilePrime);
    visitor(&m_celestialHammers);
    visitor(&m_celestialHammersPrime);
    visitor(&m_grandaxe);
    visitor(&m_grandblade);
    visitor(&m_grandhammer);
    visitor(&m_stormcallJavelin);
    visitor(&m_stormsurgeTrident);
}

Unit *Prosecutors::Create(const ParameterList &parameters)
{
    auto unit = new Prosecutors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, CelestialHammerAndShield);
    GrandWeaponOption primeGrandWeapon = (GrandWeaponOption) GetEnumParam("Prime Grand Weapon", parameters, None);
    int numTridents = GetIntParam("Stormsurge Tridents", parameters, 0);
    int numGrandaxes = GetIntParam("Grandaxes", parameters, 0);
    int numGrandhammers = GetIntParam("Grandhammers", parameters, 0);
    int numGrandblades = GetIntParam("Grandblades", parameters, 0);

    bool ok = unit->configure(numModels, weapons, primeGrandWeapon, numTridents, numGrandaxes, numGrandblades, numGrandhammers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Prosecutors::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == StormcallJavelinAndShield)
        {
            return "Stormcall Javelin and Shield";
        }
        else if (parameter.m_intValue == CelestialHammerAndShield)
        {
            return "Celestial Hammer and Shield";
        }
        else if (parameter.m_intValue == PairedCelestialHammers)
        {
            return "Paired Celestial Hammers";
        }
    }
    else if (parameter.m_name == "Prime Grand Weapon")
    {
        if (parameter.m_intValue == None)
        {
            return "None";
        }
        else if (parameter.m_intValue == StormsurgeTrident)
        {
            return "Stormsurge Trident";
        }
        else if (parameter.m_intValue == Grandaxe)
        {
            return "Grandaxe";
        }
        else if (parameter.m_intValue == Grandblade)
        {
            return "Grandblade";
        }
        else if (parameter.m_intValue == Grandhammer)
        {
            return "Grandhammer";
        }
    }

    return ParameterValueToString(parameter);
}

int Prosecutors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Stormcall Javelin and Shield")
    {
        return StormcallJavelinAndShield;
    }
    else if (enumString == "Celestial Hammer and Shield")
    {
        return CelestialHammerAndShield;
    }
    else if (enumString == "Paired Celestial Hammers")
    {
        return PairedCelestialHammers;
    }
    else if (enumString == "None")
    {
        return None;
    }
    else if (enumString == "Stormsurge Trident")
    {
        return StormsurgeTrident;
    }
    else if (enumString == "Grandaxe")
    {
        return Grandaxe;
    }
    else if (enumString == "Grandblade")
    {
        return Grandblade;
    }
    else if (enumString == "Grandhammer")
    {
        return Grandhammer;
    }
    return 0;
}

void Prosecutors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Prosecutors", factoryMethod);
    }
}

Rerolls Prosecutors::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if ((m_weapon == PairedCelestialHammers) && (weapon->name() == m_celestialHammers.name()))
    {
        return RerollOnes;
    }
    return StormcastEternal::toHitRerolls(weapon, target);
}

Rerolls Prosecutors::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    if (m_weapon == CelestialHammerAndShield || m_weapon == StormcallJavelinAndShield)
    {
        return RerollOnes;
    }
    return StormcastEternal::toSaveRerolls(weapon);
}

int Prosecutors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    if (weapon->name() == m_grandaxe.name())
    {
        // count the number of models in the unit w/in 1 in of this model
        return target->numModelsWithin(attackingModel, weapon->range());
    }
    return StormcastEternal::extraAttacks(nullptr, weapon, target);
}

} //namespace StormcastEternals