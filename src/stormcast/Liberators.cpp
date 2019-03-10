/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/Liberators.h>
#include <iostream>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    Liberators::Create,
    Liberators::ValueToString,
    Liberators::EnumStringToInt,
    {
        {ParamType::Integer, "Models", {.m_intValue = 5}, Liberators::MIN_UNIT_SIZE, Liberators::MAX_UNIT_SIZE, Liberators::MIN_UNIT_SIZE},
        {
            ParamType::Enum, "Weapons", {.m_intValue = Liberators::Warhammer}, Liberators::Warhammer,
            Liberators::Warblade, 1
        },
        {ParamType::Boolean, "Paired Weapons", {.m_boolValue = false}, false, false},
        {ParamType::Integer, "Grandhammers", {.m_intValue = 0}, 0, Liberators::MAX_UNIT_SIZE / 5, 1},
        {ParamType::Integer, "Grandblades", {.m_intValue = 0}, 0, Liberators::MAX_UNIT_SIZE / 5, 1}
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Liberators::s_registered = false;

Liberators::Liberators() :
    StormcastEternal("Liberators", 5, WOUNDS, 7, 4, false),
    m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1),
    m_warhammerPrime(Weapon::Type::Melee, "Warhammer (Prime)", 1, 3, 4, 3, 0, 1),
    m_warblade(Weapon::Type::Melee, "Warblade", 1, 2, 3, 4, 0, 1),
    m_warbladePrime(Weapon::Type::Melee, "Warblade (Prime)", 1, 3, 3, 4, 0, 1),
    m_grandhammer(Weapon::Type::Melee, "Grandhammer", 1, 2, 4, 3, -1, 2),
    m_grandblade(Weapon::Type::Melee, "Grandblade", 1, 2, 3, 4, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, REDEEMER, LIBERATORS};
}

bool
Liberators::configure(int numModels, WeaponOption weapons, bool pairedWeapons, int numGrandhammers, int numGrandblades)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGrandweapons = numModels / 5;
    if (numGrandblades + numGrandhammers > maxGrandweapons)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_weaponOption = weapons;
    m_pairedWeapons = pairedWeapons;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (m_weaponOption == Warhammer)
    {
        primeModel.addMeleeWeapon(&m_warhammerPrime);
    }
    else if (m_weaponOption == Warblade)
    {
        primeModel.addMeleeWeapon(&m_warbladePrime);
    }
    addModel(primeModel);

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
        if (m_weaponOption == Warhammer)
        {
            model.addMeleeWeapon(&m_warhammer);
        }
        else if (m_weaponOption == Warblade)
        {
            model.addMeleeWeapon(&m_warblade);
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

int Liberators::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = StormcastEternal::toHitModifier(weapon, unit);

    // Lay Low the Tyrants
    if (unit->wounds() >= 5)
    {
        modifier += 1;
    }

    return modifier;
}

Rerolls Liberators::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    if (m_weaponOption == Warhammer || m_weaponOption == Warblade)
    {
        return RerollOnes;
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

int Liberators::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    if (unmodifiedHitRoll == 6)
    {
        if (m_pairedWeapons && (weapon->name() == m_warblade.name() || weapon->name() == m_warhammer.name()))
        {
            // each 6 inflicts an additional hit
            return 2;
        }
    }
    return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
}

Unit *Liberators::Create(const ParameterList &parameters)
{
    auto libs = new Liberators();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Warblade);
    bool pairedWeapons = GetBoolParam("Paired Weapons", parameters, false);
    int numGrandhammers = GetIntParam("Grandhammers", parameters, 0);
    int numGrandblades = GetIntParam("Grandblades", parameters, 0);

    bool ok = libs->configure(numModels, weapons, pairedWeapons, numGrandhammers, numGrandblades);
    if (!ok)
    {
        delete libs;
        libs = nullptr;
    }
    return libs;
}

void Liberators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Liberators", factoryMethod);
    }
}

std::string Liberators::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Warhammer)
        {
            return "Warhammer";
        }
        else if (parameter.m_intValue == Warblade)
        {
            return "Warblade";
        }
    }

    return ParameterValueToString(parameter);
}

int Liberators::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Warhammer")
    {
        return Warhammer;
    }
    else if (enumString == "Warblade")
    {
        return Warblade;
    }
    return 0;
}

void Liberators::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_grandblade);
    visitor(&m_grandhammer);
    visitor(&m_warhammer);
    visitor(&m_warhammerPrime);
    visitor(&m_warblade);
    visitor(&m_warbladePrime);
}

} // namespace StormcastEternals
