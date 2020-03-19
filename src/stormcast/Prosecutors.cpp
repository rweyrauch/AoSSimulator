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

bool Prosecutors::s_registered = false;

Prosecutors::Prosecutors() :
    StormcastEternal("Prosecutors", 12, WOUNDS, 7, 4, true),
    m_celestialHammersMissile(Weapon::Type::Missile, "Celestial Hammer(s)", 18, 2, 4, 4, 0, 1),
    m_stormcallJavelinMissile(Weapon::Type::Missile, "Stormcall Javelin", 18, 1, 3, 3, 0, 1),
    m_stormcallJavelinMissilePrime(Weapon::Type::Missile, "Stormcall Javelin", 18, 2, 3, 3, 0, 1),
    m_stormsurgeTridentMissile(Weapon::Type::Missile, "Stormsurge Trident", 18, 1, 3, 3, -1, 2),
    m_stormsurgeTridentMissilePrime(Weapon::Type::Missile, "Stormsurge Trident", 18, 2, 3, 3, -1, 2),
    m_celestialHammers(Weapon::Type::Melee, "Celestial Hammers(s)", 1, 2, 3, 3, 0, 1),
    m_celestialHammersPrime(Weapon::Type::Melee, "Celestial Hammers(s)", 1, 3, 3, 3, 0, 1),
    m_grandaxe(Weapon::Type::Melee, "Grandaxe", 1, 0, 3, 3, -1, 1),
    m_grandblade(Weapon::Type::Melee, "Grandblade", 1, 2, 3, 4, -1, 2),
    m_grandhammer(Weapon::Type::Melee, "Grandhammer", 1, 2, 4, 3, -1, 2),
    m_stormcallJavelin(Weapon::Type::Melee, "Stormcall Javelin", 2, 1, 4, 4, 0, 1),
    m_stormsurgeTrident(Weapon::Type::Melee, "Stormsurge Trident", 2, 1, 4, 4, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, ANGELOS, PROSECUTORS};
    m_weapons = {&m_celestialHammersMissile,
                 &m_stormcallJavelinMissile,
                 &m_stormcallJavelinMissilePrime,
                 &m_stormsurgeTridentMissile,
                 &m_stormsurgeTridentMissilePrime,
                 &m_celestialHammers,
                 &m_celestialHammersPrime,
                 &m_grandaxe,
                 &m_grandblade,
                 &m_grandhammer,
                 &m_stormcallJavelin,
                 &m_stormsurgeTrident};
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

    m_weaponOption = weapons;

    auto prime = new Model(BASESIZE, WOUNDS);
    switch (primeGrandWeapon)
    {
        case None:
            if (weapons == CelestialHammerAndShield || weapons == PairedCelestialHammers)
            {
                prime->addMissileWeapon(&m_celestialHammersMissile);
                prime->addMeleeWeapon(&m_celestialHammersPrime);
            }
            else if (weapons == StormcallJavelinAndShield)
            {
                prime->addMissileWeapon(&m_stormcallJavelinMissilePrime);
                prime->addMeleeWeapon(&m_stormcallJavelin);
            }
            break;
        case StormsurgeTrident:
            prime->addMissileWeapon(&m_stormsurgeTridentMissilePrime);
            prime->addMeleeWeapon(&m_stormsurgeTrident);
            totalTridents--;
            break;
        case Grandaxe:
            prime->addMeleeWeapon(&m_grandaxe);
            numGrandaxes--;
            break;
        case Grandblade:
            prime->addMeleeWeapon(&m_grandblade);
            numGrandblades--;
            break;
        case Grandhammer:
            prime->addMeleeWeapon(&m_grandhammer);
            numGrandhammers--;
            break;
    }
    addModel(prime);

    for (auto i = 0; i < totalTridents; i++)
    {
        auto tridentModel = new Model(BASESIZE, WOUNDS);
        tridentModel->addMissileWeapon(&m_stormsurgeTridentMissile);
        tridentModel->addMeleeWeapon(&m_stormsurgeTrident);
        addModel(tridentModel);
    }
    for (auto i = 0; i < numGrandaxes; i++)
    {
        auto grandaxeModel = new Model(BASESIZE, WOUNDS);
        grandaxeModel->addMeleeWeapon(&m_grandaxe);
        addModel(grandaxeModel);
    }
    for (auto i = 0; i < numGrandblades; i++)
    {
        auto grandbladeModel = new Model(BASESIZE, WOUNDS);
        grandbladeModel->addMeleeWeapon(&m_grandblade);
        addModel(grandbladeModel);
    }
    for (auto i = 0; i < numGrandhammers; i++)
    {
        auto grandhammerModel = new Model(BASESIZE, WOUNDS);
        grandhammerModel->addMeleeWeapon(&m_grandhammer);
        addModel(grandhammerModel);
    }
    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (m_weaponOption == CelestialHammerAndShield || m_weaponOption == PairedCelestialHammers)
        {
            model->addMissileWeapon(&m_celestialHammersMissile);
            model->addMeleeWeapon(&m_celestialHammers);
        }
        else if (m_weaponOption == StormcallJavelinAndShield)
        {
            model->addMissileWeapon(&m_stormcallJavelinMissile);
            model->addMeleeWeapon(&m_stormcallJavelin);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

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

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

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
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == StormcallJavelinAndShield)
        {
            return "Stormcall Javelin and Shield";
        }
        else if (parameter.intValue == CelestialHammerAndShield)
        {
            return "Celestial Hammer and Shield";
        }
        else if (parameter.intValue == PairedCelestialHammers)
        {
            return "Paired Celestial Hammers";
        }
    }
    else if (std::string(parameter.name) == "Prime Grand Weapon")
    {
        if (parameter.intValue == None)
        {
            return "None";
        }
        else if (parameter.intValue == StormsurgeTrident)
        {
            return "Stormsurge Trident";
        }
        else if (parameter.intValue == Grandaxe)
        {
            return "Grandaxe";
        }
        else if (parameter.intValue == Grandblade)
        {
            return "Grandblade";
        }
        else if (parameter.intValue == Grandhammer)
        {
            return "Grandhammer";
        }
    }

    return StormcastEternal::ValueToString(parameter);
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
    return StormcastEternal::EnumStringToInt(enumString);
}

void Prosecutors::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Weapons", StormcallJavelinAndShield, StormcallJavelinAndShield, CelestialHammerAndShield, 1},
                {ParamType::Enum, "Prime Grand Weapon", None, None, Grandhammer, 1},
                {ParamType::Integer, "Stormsurge Tridents", 0, 0, MAX_UNIT_SIZE/3, 1 },
                {ParamType::Integer, "Grandaxes", 0, 0, MAX_UNIT_SIZE/3, 1 },
                {ParamType::Integer, "Grandblades", 0, 0, MAX_UNIT_SIZE/3, 1 },
                {ParamType::Integer, "Grandhammers", 0, 0, MAX_UNIT_SIZE/3, 1 },
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };
        s_registered = UnitFactory::Register("Prosecutors", factoryMethod);
    }
}

Rerolls Prosecutors::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if ((m_weaponOption == PairedCelestialHammers) && (weapon->name() == m_celestialHammers.name()))
    {
        return RerollOnes;
    }
    return StormcastEternal::toHitRerolls(weapon, target);
}

Rerolls Prosecutors::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    if (m_weaponOption == CelestialHammerAndShield || m_weaponOption == StormcallJavelinAndShield)
    {
        return RerollOnes;
    }
    return StormcastEternal::toSaveRerolls(weapon);
}

int Prosecutors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = StormcastEternal::extraAttacks(nullptr, weapon, target);
    if (weapon->name() == m_grandaxe.name())
    {
        // count the number of models in the unit w/in 1 in of this model
        extra += target->numModelsWithin(attackingModel, (float)weapon->range());
    }
    return extra;
}

int Prosecutors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace StormcastEternals
